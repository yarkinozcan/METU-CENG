#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include "parser.h"
#include "ppm.h"

using namespace parser;

double custom_epsilon = 1e-12;

// Utility functions for vector operations with operator overloading
Vec3f operator+(const Vec3f& a, const Vec3f& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3f operator-(const Vec3f& a, const Vec3f& b) { // Subtraction of two vectors
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3f operator*(const Vec3f& v, float scalar) { // Multiplication of a vector by a scalar
    return {v.x * scalar, v.y * scalar, v.z * scalar};
}

Vec3f operator*(float scalar, const Vec3f& v) { // Multiplication of a scalar by a vector
    return {v.x * scalar, v.y * scalar, v.z * scalar};
}

Vec3f operator*(const Vec3f& a, const Vec3f& b) { // Element-wise multiplication of two vectors
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}

float dot(const Vec3f& a, const Vec3f& b) { // Dot product of two vectors
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3f cross(const Vec3f& a, const Vec3f& b) { // Cross product of two vectors
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float length(const Vec3f& v) { // Length of the vector
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3f normalize(const Vec3f& v) { // Normalize the vector
    float len = length(v);
    if (len == 0) return {0, 0, 0};
    return {v.x / len, v.y / len, v.z / len};
}

struct Ray { // Ray struct
    Vec3f origin; // Origin of the ray
    Vec3f direction; // Direction of the ray
};

struct HitInfo { // Hit information struct
    bool hit; // Boolean to check if there is a hit
    float t; // Distance to the hit point
    Vec3f point; // Hit point
    Vec3f normal; // Normal at the hit point
    int material_id; // Material ID of the hit object
};

// Thread data structure for parallel rendering -> Datas that we give to each thread before they process their rows
struct ThreadData {
    const Scene* scene;          // Shared scene (read-only)
    const Camera* camera;        // Camera parameters
    unsigned char* image;         // Shared image buffer
    int start_row;               // First row this thread renders
    int end_row;                 // Last row (exclusive)
    int width;                   // Image width
    int height;                  // Image height
    Vec3f gaze, v, w, u;        // Camera vectors
    Vec3f q;                     // Image plane origin
    Vec3f pixelStepX, pixelStepY; // Pixel stepping vectors
};

// Ray-sphere intersection
HitInfo find_intersecting_sphere(const Ray& ray, const Sphere& sphere, const Vec3f& center, const Scene& scene) {
    HitInfo hit; // Initialize the hitInfo struct
    hit.hit = false; // Initialize the hit to false
    
    // (p - c) · (p - c) - R² = 0
    // where c = center, R = radius
    Vec3f c = center;  // sphere center
    float R = sphere.radius;  // sphere radius
    
    // (o + td - c) · (o + td - c) - R² = 0
    // Expanding: (d·d)t² + 2d·(o-c)t + (o-c)·(o-c) - R² = 0
    Vec3f oc = ray.origin - c;  // (o - c)
    float a = dot(ray.direction, ray.direction);  // d·d
    float b = 2.0f * dot(oc, ray.direction);  // 2d·(o-c)
    float c_coeff = dot(oc, oc) - R * R;  // (o-c)·(o-c) - R²
    
    float discriminant = b * b - 4 * a * c_coeff;  // b² - 4ac
    
    // if discriminant >= 0, there are solutions
    if (discriminant >= 0) {
        float t1 = (-b - sqrt(discriminant)) / (2 * a); 
        float t2 = (-b + sqrt(discriminant)) / (2 * a);
        
        // Choose the closest valid intersection
        float t = (t1 > custom_epsilon) ? t1 : t2;
        if (t > custom_epsilon) {
            hit.hit = true;
            hit.t = t;
            hit.point = ray.origin + ray.direction * t;  // r(t) = o + td
            hit.normal = normalize(hit.point - c);  // (p - c) / |p - c|
            hit.material_id = sphere.material_id;
        }
    }
    
    return hit;
}

// Ray-triangle intersection using barycentric coordinates
HitInfo find_intersecting_triangle(const Ray& ray, const Face& face, const std::vector<Vec3f>& vertices, const Scene& scene, bool useBackFaceCulling = true) {
    HitInfo hit;
    hit.hit = false;
    
    // triangle vertices a, b, c
    Vec3f a = vertices[face.v0_id - 1]; // Convert to 0-based indexing
    Vec3f b = vertices[face.v1_id - 1];
    Vec3f c = vertices[face.v2_id - 1];
    
    // ray r(t) = o + td
    Vec3f o = ray.origin;  // ray origin
    Vec3f d = ray.direction;  // ray direction 
    
    // Back-face culling: Check if triangle normal points away from camera

    if (useBackFaceCulling) {
        Vec3f triangleNormal = normalize(cross(b - a, c - a));
        float dotProduct = dot(d, triangleNormal);
        // If dot product is positive, triangle is facing away from camera
        if (dotProduct > 0) {
            return hit; // Skip this triangle
        }
    }
    
    // matrix A = [a-b, a-c, d]
    // Calculate matrix elements
    float ax_bx = a.x - b.x;  // (a-b).x
    float ax_cx = a.x - c.x;  // (a-c).x
    float dx = d.x;           // d.x
    
    float ay_by = a.y - b.y;  // (a-b).y
    float ay_cy = a.y - c.y;  // (a-c).y
    float dy = d.y;           // d.y
    
    float az_bz = a.z - b.z;  // (a-b).z
    float az_cz = a.z - c.z;  // (a-c).z
    float dz = d.z;           // d.z
    
    // |A| = a(ei - hf) + b(gf - di) + c(dh - eg)
    float det_A = ax_bx * (ay_cy * dz - dy * az_cz) + 
                  ay_by * (dx * az_cz - ax_cx * dz) + 
                  az_bz * (ax_cx * dy - ay_cy * dx);
    
    // if |A| = 0, ray is parallel to triangle
    if (std::abs(det_A) < custom_epsilon) return hit;
    
    // right-hand side [a-o]
    float ax_ox = a.x - o.x;  // (a-o).x
    float ay_oy = a.y - o.y;  // (a-o).y
    float az_oz = a.z - o.z;  // (a-o).z
    
    // beta = |A_1|/|A| using Cramer's rule
    // A_1 is A with first column replaced by [a-o]
    float det_A1 = ax_ox * (ay_cy * dz - dy * az_cz) + 
                   ay_oy * (dx * az_cz - ax_cx * dz) + 
                   az_oz * (ax_cx * dy - ay_cy * dx);
    
    float beta = det_A1 / det_A;
    
    //  γ = |A_2|/|A| using Cramer's rule
    // A_2 is A with second column replaced by [a-o]
    float det_A2 = ax_bx * (ay_oy * dz - dy * az_oz) + 
                   ay_by * (dx * az_oz - ax_ox * dz) + 
                   az_bz * (ax_ox * dy - ay_oy * dx);
    
    float gamma = det_A2 / det_A;
    
    // t = |A_3|/|A| using Cramer's rule
    // A_3 is A with third column replaced by [a-o]
    float det_A3 = ax_bx * (ay_cy * az_oz - ay_oy * az_cz) + 
                   ay_by * (ax_ox * az_cz - ax_cx * az_oz) + 
                   az_bz * (ax_cx * ay_oy - ay_cy * ax_ox);
    
    float t = det_A3 / det_A;
    
    // barycentric coordinates must satisfy beta ≥ 0, gamma ≥ 0, beta + gamma ≤ 1
    if (beta < 0.0f || gamma < 0.0f || beta + gamma > 1.0f) return hit;
    
    // Check if intersection is in front of ray origin
    if (t > custom_epsilon) {
        hit.hit = true;
        hit.t = t;
        hit.point = o + d * t;  // r(t) = o + td
        hit.normal = normalize(cross(b - a, c - a));  // (b-a) × (c-a)
        hit.material_id = 0; // Will be set by caller
    }
    
    return hit;
}

// Ray-plane intersection
HitInfo find_intersecting_plane(const Ray& ray, const Plane& plane, const Vec3f& center, const Scene& scene) {
    HitInfo hit;
    hit.hit = false;
    
    // (p - a) · n = 0
    // where a = center (point on plane), n = plane.normal
    Vec3f a = center;  // point on plane
    Vec3f n = plane.normal;  // plane normal 
    
    // d · n (ray direction dot normal)
    float denom = dot(ray.direction, n);
    
    // if d · n = 0, ray is parallel to plane
    if (std::abs(denom) > custom_epsilon) {
        // t = (a - o) · n / (d · n)
        Vec3f ao = a - ray.origin;  // (a - o)
        float t = dot(ao, n) / denom;        // (a - o) · n / (d · n)
        
        // Check if intersection is in front of ray origin
        if (t > custom_epsilon) {
            hit.hit = true;
            hit.t = t;
            hit.point = ray.origin + ray.direction * t;  // r(t) = o + td
            if (denom > 0) { // Ray is hitting the back of the plane
                hit.normal = normalize(n * -1.0f); // Flip the normal
            } else { // Ray is hitting the front of the plane
                hit.normal = normalize(n);
            }
            hit.material_id = plane.material_id;
        }
    }
    
    return hit;
}

// Ray-cylinder intersection
HitInfo find_intersecting_cylinder(const Ray& ray, const Cylinder& cylinder, const Vec3f& center, const Scene& scene) {
    HitInfo hit;
    hit.hit = false;
    
    // cylinder axis and radius
    Vec3f axis = normalize(cylinder.axis);  // cylinder axis
    float R = cylinder.radius;  // cylinder radius
    
    // ray r(t) = o + td
    Vec3f o = ray.origin;  // ray origin
    Vec3f d = ray.direction;  // ray direction 
    Vec3f c = center;  // cylinder center 
    
    // oc = o - c
    Vec3f oc = o - c;
    
    // Side test (infinite tube)
    // For infinite cylinder: (d - (d·a)a) · (d - (d·a)a) t² + 2(d - (d·a)a) · (oc - (oc·a)a) t + (oc - (oc·a)a) · (oc - (oc·a)a) - R² = 0
    float a = dot(d, d) - dot(d, axis) * dot(d, axis);  // d·d - (d·a)²
    float b = 2.0f * (dot(oc, d) - dot(oc, axis) * dot(d, axis));  // 2(oc·d - (oc·a)(d·a))
    float c_coeff = dot(oc, oc) - dot(oc, axis) * dot(oc, axis) - R * R;  // oc·oc - (oc·a)² - R²
    
    float discriminant = b * b - 4 * a * c_coeff;  // b² - 4ac
    float t_side = std::numeric_limits<float>::max();
    
    if (discriminant >= 0) {
        float t1 = (-b - sqrt(discriminant)) / (2 * a); 
        float t2 = (-b + sqrt(discriminant)) / (2 * a);
        
        for (float t : {t1, t2}) {
            if (t > custom_epsilon) {
                Vec3f point = o + d * t;  // r(t) = o + td
                Vec3f toPoint = point - c;
                float height = dot(toPoint, axis);
                
                // Check if point is within cylinder height bounds
                if (height >= -cylinder.height/2 && height <= cylinder.height/2) {
                    t_side = std::min(t_side, t);
                }
            }
        }
    }
    
    // Cap tests (two disks)
    float t_cap = std::numeric_limits<float>::max();
    Vec3f cap_normal = axis;  // cap normal
    
    for (int cap = 0; cap < 2; cap++) {
        float cap_height = (cap == 0) ? -cylinder.height/2 : cylinder.height/2;
        Vec3f cap_center = c + axis * cap_height;  // cap center
        
        // plane intersection t = (a - o) · n / (d · n)
        float denom = dot(cap_normal, d);  // d · n
        if (std::abs(denom) > custom_epsilon) {
            Vec3f oc_cap = cap_center - o;  // (a - o)
            float t = dot(oc_cap, cap_normal) / denom;  // (a - o) · n / (d · n)
            
            if (t > custom_epsilon) {
                Vec3f point = o + d * t;  // r(t) = o + td
                Vec3f toPoint = point - cap_center;
                float dist_to_center = length(toPoint);
                
                if (dist_to_center <= R) {
                    t_cap = std::min(t_cap, t);
                }
            }
        }
    }
    
    // Choose the earliest valid hit
    float t = std::min(t_side, t_cap);
    if (t < std::numeric_limits<float>::max()) {
        hit.hit = true;
        hit.t = t;
        hit.point = o + d * t;  // r(t) = o + td
        
        Vec3f toPoint = hit.point - c;
        float height = dot(toPoint, axis);
        
        if (t == t_side) {
            // Lateral hit - normal perpendicular to axis
            Vec3f projection = axis * height;
            hit.normal = normalize(toPoint - projection);
        } else {
            // Cap hit - normal along axis
            hit.normal = (height < 0) ? axis * -1.0f : axis;
        }
        
        hit.material_id = cylinder.material_id;
    }
    
    return hit;
}

// Find closest intersection
HitInfo find_closest_intersecting_object(const Ray& ray, const Scene& scene) {
    HitInfo closestHit; // obj = NULL
    closestHit.hit = false; // initialzie with no hit
    closestHit.t = std::numeric_limits<float>::max(); // t_min = infinity
    
    // Check spheres
    for (const auto& sphere : scene.spheres) {
        Vec3f center = scene.vertex_data[sphere.center_vertex_id - 1];
        HitInfo hit = find_intersecting_sphere(ray, sphere, center, scene);
        if (hit.hit && hit.t < closestHit.t) {
            closestHit = hit;
        }
    }
    
    // Check triangles (with back-face culling for primary rays)
    for (const auto& triangle : scene.triangles) {
        HitInfo hit = find_intersecting_triangle(ray, triangle.indices, scene.vertex_data, scene, true);
        if (hit.hit && hit.t < closestHit.t) {
            hit.material_id = triangle.material_id;
            closestHit = hit;
        }
    }
    
    // Check mesh faces (with back-face culling for primary rays)
    for (const auto& mesh : scene.meshes) {
        for (const auto& face : mesh.faces) {
            HitInfo hit = find_intersecting_triangle(ray, face, scene.vertex_data, scene, true);
            if (hit.hit && hit.t < closestHit.t) {
                hit.material_id = mesh.material_id;
                closestHit = hit;
            }
        }
    }
    
    // Check planes
    for (const auto& plane : scene.planes) {
        Vec3f center = scene.vertex_data[plane.center_vertex_id - 1];
        HitInfo hit = find_intersecting_plane(ray, plane, center, scene);
        if (hit.hit && hit.t < closestHit.t) {
            closestHit = hit;
        }
    }
    
    // Check cylinders
    for (const auto& cylinder : scene.cylinders) {
        Vec3f center = scene.vertex_data[cylinder.center_vertex_id - 1];
        HitInfo hit = find_intersecting_cylinder(ray, cylinder, center, scene);
        if (hit.hit && hit.t < closestHit.t) {
            closestHit = hit;
        }
    }
    
    return closestHit;
}

// Find any intersection (for shadow rays - early termination)
bool findIntersectionForShadowRay(const Ray& ray, const Scene& scene, float maxDistance) {

    double custom_epsilon_shadow = scene.shadow_ray_epsilon;

    // Check spheres
    for (const auto& sphere : scene.spheres) {
        Vec3f center = scene.vertex_data[sphere.center_vertex_id - 1];
        HitInfo hit = find_intersecting_sphere(ray, sphere, center, scene);
        if (hit.hit && hit.t > custom_epsilon_shadow && hit.t < maxDistance) {
            return true; // Early termination for shadow rays
        }
    }

    /*
        Here we do not make back-face culling for shadow rays because we want to check all triangles. If we make back-face culling, than we will miss some triangles that could prevent the point from receiving light
    */
    
    // Check triangles (no back-face culling for shadow rays)
    for (const auto& triangle : scene.triangles) {
        HitInfo hit = find_intersecting_triangle(ray, triangle.indices, scene.vertex_data, scene, false);
        if (hit.hit && hit.t > custom_epsilon_shadow && hit.t < maxDistance) {
            return true;
        }
    }
    
    // Check mesh faces (no back-face culling for shadow rays)
    for (const auto& mesh : scene.meshes) {
        for (const auto& face : mesh.faces) {
            HitInfo hit = find_intersecting_triangle(ray, face, scene.vertex_data, scene, false);
            if (hit.hit && hit.t > custom_epsilon_shadow && hit.t < maxDistance) {
                return true;
            }
        }
    }
    
    // Check planes
    for (const auto& plane : scene.planes) {
        Vec3f center = scene.vertex_data[plane.center_vertex_id - 1];
        HitInfo hit = find_intersecting_plane(ray, plane, center, scene);
        if (hit.hit && hit.t > custom_epsilon_shadow && hit.t < maxDistance) {
            return true;
        }
    }
    
    // Check cylinders
    for (const auto& cylinder : scene.cylinders) {
        Vec3f center = scene.vertex_data[cylinder.center_vertex_id - 1];
        HitInfo hit = find_intersecting_cylinder(ray, cylinder, center, scene);
        if (hit.hit && hit.t > custom_epsilon_shadow && hit.t < maxDistance) {
            return true;
        }
    }
    
    return false;
}

// Check if point is in shadow
bool is_in_shadow(const Vec3f& point, const Vec3f& lightPos, const Vec3f& normal, const Scene& scene) {
    Vec3f lightDir = lightPos - point;
    float lightDist = length(lightDir);
    
    // If light is at the same position as the point, it's not in shadow
    if (lightDist < scene.shadow_ray_epsilon) return false;
    
    lightDir = normalize(lightDir);
    
    Ray shadowRay;
    // Offset along surface normal to avoid shadow acne
    shadowRay.origin = point + normal * scene.shadow_ray_epsilon;
    shadowRay.direction = lightDir;
    
    // Use optimized shadow ray testing with early termination
    return findIntersectionForShadowRay(shadowRay, scene, lightDist);
}

// Calculate the corresponding color and shading for the hit object
Vec3f calculateColorAndShading(const HitInfo& hit, const Ray& ray, const Scene& scene) {
    const Material& material = scene.materials[hit.material_id - 1]; // Get the material of the hit object
    
    // Ambient component
    Vec3f color = material.ambient * scene.ambient_light;
    
    for (const auto& light : scene.point_lights) { // For each point light
        if (!is_in_shadow(hit.point, light.position, hit.normal, scene)) { // Here we check that if our hit point is not affected by each light source. If our points is not in shadow, than we can calculate the color and shading for the hit object for that specific light source
            Vec3f lightDir = normalize(light.position - hit.point); // Light direction
            Vec3f viewDir = normalize(ray.origin - hit.point); // View direction
            Vec3f halfVector = normalize(lightDir + viewDir); // Half vector
            
            // Received irradiance: E(d) = I / d^2
            float distance = length(light.position - hit.point);
            Vec3f irradiance = light.intensity * (1.0f / (distance * distance)); // I_p = I / d^2
            
            // Diffuse component
            float diffuse = std::max(0.0f, dot(hit.normal, lightDir)); // cos theta = max(0, w_i . n)
            Vec3f diffuseColor = material.diffuse * irradiance * diffuse; // k_d * I_p * cos theta
            
            // Specular component (Blinn-Phong)
            float cosAlpha = std::max(0.0f, dot(hit.normal, halfVector)); // cos alpha = max(0, n . h)
            float cosAlphaPower = pow(cosAlpha, material.phong_exponent); // cos alpha ^ phong exponent
            Vec3f specularColor = material.specular * irradiance * cosAlphaPower; // k_s * I_p * (cos alpha ^ phong exponent)
            
            color = color + diffuseColor;
            color = color + specularColor;
        }
    }
    
    return color;
}

Vec3f follow_ray(const Ray& ray, const Scene& scene, int depth) {
    if (depth < 0) {
        return {0, 0, 0}; // Stop recursion and return black
    }

    HitInfo hit = find_closest_intersecting_object(ray, scene);

    Vec3f color;

    if (!hit.hit) {
        // Return background color ONLY for primary rays. (so that we do not return background color multiple times)
        if (depth == scene.max_recursion_depth) {
            color.x = (float)scene.background_color.x;
            color.y = (float)scene.background_color.y;
            color.z = (float)scene.background_color.z;
        }
        // Reflected rays that miss should be black.
        else {
            color = {0, 0, 0};
        }
        return color;
    }

    color = calculateColorAndShading(hit, ray, scene);

    const Material& material = scene.materials[hit.material_id - 1];
    if (material.is_mirror) {
        // w_r = -w_o + 2n(n · w_o)
        Vec3f wo = -1.0f * ray.direction;  // Outgoing direction (the opposite of the direction of the ray)
        Vec3f n = hit.normal; // Surface normal
        float n_dot_wo = dot(n, wo);  // n · w_o
        Vec3f reflectDir = n * (2.0f * n_dot_wo) - wo;  // w_r = 2n(n · w_o) - w_o 
        Ray reflectRay;
        // Offset along surface normal to avoid reflection acne
        reflectRay.origin = hit.point + hit.normal * scene.shadow_ray_epsilon;
        reflectRay.direction = reflectDir;
        
        Vec3f reflectColor = follow_ray(reflectRay, scene, depth - 1);
        color = color + material.mirror * reflectColor;
    }

    return color;
}

// Worker function for each thread
void* render_pixel_rows(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    // Render assigned rows
    #pragma omp parallel for
    for (int j = data->start_row; j < data->end_row; ++j) {
        for (int i = 0; i < data->width; ++i) {
            Vec3f pixelPos = data->q + data->pixelStepX * (i + 0.5) + data->pixelStepY * (j + 0.5); // pixel position s(i, j)
            
            Ray ray; // Get the viewing ray for the pixel (i, j)
            ray.origin = data->camera->position; // Origin of the ray is the position of the camera
            ray.direction = normalize(pixelPos - data->camera->position); // Direction of the ray is the normalized difference between the pixel position and the camera position
            
            Vec3f color = follow_ray(ray, *data->scene, data->scene->max_recursion_depth);
            
            // Clamp color values to [0, 255]
            color.x = std::min(255.0f, std::max(0.0f, color.x));
            color.y = std::min(255.0f, std::max(0.0f, color.y));
            color.z = std::min(255.0f, std::max(0.0f, color.z));
            
            int index = (j * data->width + i) * 3;
            data->image[index] = (unsigned char)color.x;
            data->image[index + 1] = (unsigned char)color.y;
            data->image[index + 2] = (unsigned char)color.z;
        }
    }
    
    return nullptr;
}

int main(int argc, char* argv[])
{
    Scene scene;
    scene.loadFromXml(argv[1]);

    // Process each camera
    for (const auto& camera : scene.cameras) {
        // Image resolution values
        int width = camera.image_width;
        int height = camera.image_height;
        
        // Image pixel data (Each pixel having R, G, B values)
        unsigned char* image = new unsigned char[width * height * 3];
        
        // Calculate camera basis vectors (right-handed coordinate system)
        Vec3f gaze = normalize(camera.gaze); // Opposite of vector w (had to do normalization here since in some cases gaze vector is not normalized)
        Vec3f v = normalize(camera.up); // vector v
        Vec3f w = normalize(gaze * -1.0f);  // w = -gaze
        Vec3f u = cross(v, w); // Vector u found by cross product of vector w and v

        // Calculate image plane corners
        // Near plane format: (Left, Right, Bottom, Top)
        Vec3f nearCenter = camera.position + gaze * camera.near_distance; // the value of m (center of the image plane)
        Vec3f q = nearCenter + v * camera.near_plane.w + u * camera.near_plane.x; // the value of q (found by q = m + top * v + left * u) here near_plane.w is Top and near_plane.x is Left

        float s_u = (camera.near_plane.y - camera.near_plane.x) / width; // 1 pixel length in X direction
        float s_v = (camera.near_plane.z - camera.near_plane.w) / height; // 1 pixel length in Y direction

        Vec3f pixelStepX = u * s_u; // Vector used to step in X direction
        Vec3f pixelStepY = v * s_v; // Vector used to step in Y direction

        // Multi-threaded rendering
        int max_threads = 16;
        const int num_threads = std::min(max_threads, height); // Use up to max_threads or height, whichever is smaller
        std::vector<pthread_t> threads(num_threads);
        std::vector<ThreadData> thread_data(num_threads);
        
        // Calculate rows per thread
        int rows_per_thread = height / num_threads;
        int remaining_rows = height % num_threads;
        
        // Create and start threads
        int current_row = 0;
        for (int t = 0; t < num_threads; ++t) {
            thread_data[t].scene = &scene;
            thread_data[t].camera = &camera;
            thread_data[t].image = image;
            thread_data[t].start_row = current_row;
            thread_data[t].end_row = current_row + rows_per_thread + (t < remaining_rows ? 1 : 0);
            thread_data[t].width = width;
            thread_data[t].height = height;
            thread_data[t].gaze = gaze;
            thread_data[t].v = v;
            thread_data[t].w = w;
            thread_data[t].u = u;
            thread_data[t].q = q;
            thread_data[t].pixelStepX = pixelStepX;
            thread_data[t].pixelStepY = pixelStepY;
            
            pthread_create(&threads[t], nullptr, render_pixel_rows, &thread_data[t]);
            current_row = thread_data[t].end_row;
        }
        
        // Wait for all threads to complete
        for (int t = 0; t < num_threads; ++t) {
            pthread_join(threads[t], nullptr);
        }
        
        write_ppm(camera.image_name.c_str(), image, width, height);
        delete[] image;
    }
    
    return 0;
}
