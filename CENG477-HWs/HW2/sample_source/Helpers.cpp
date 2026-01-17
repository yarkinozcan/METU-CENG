#include <iostream>
#include <cmath>
#include "Helpers.h"

/*
 * Calculate cross product of vec3 a, vec3 b and return resulting vec3.
 */
Vec3 crossProductVec3(Vec3 a, Vec3 b)
{
    return Vec3(a.y * b.z - b.y * a.z, b.x * a.z - a.x * b.z, a.x * b.y - b.x * a.y);
}

/*
 * Calculate dot product of vec3 a, vec3 b and return resulting value.
 */
double dotProductVec3(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*
 * Find length (|v|) of vec3 v.
 */
double magnitudeOfVec3(Vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/*
 * Normalize the vec3 to make it unit vec3.
 */
Vec3 normalizeVec3(Vec3 v)
{
    double d = magnitudeOfVec3(v);
    return Vec3(v.x / d, v.y / d, v.z / d);
}

/*
 * Return -v (inverse of vec3 v)
 */
Vec3 inverseVec3(Vec3 v)
{
    return Vec3(-v.x, -v.y, -v.z);
}

/*
 * Add vec3 a to vec3 b and return resulting vec3 (a+b).
 */
Vec3 addVec3(Vec3 a, Vec3 b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/*
 * Subtract vec3 b from vec3 a and return resulting vec3 (a-b).
 */
Vec3 subtractVec3(Vec3 a, Vec3 b)
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

/*
 * Multiply each element of vec3 with scalar.
 */
Vec3 multiplyVec3WithScalar(Vec3 v, double c)
{
    return Vec3(v.x * c, v.y * c, v.z * c);
}

/*
 * Prints elements in a vec3. Can be used for debugging purposes.
 */
void printVec3(Vec3 v)
{
    std::cout << "(" << v.x << "," << v.y << "," << v.z << ")" << std::endl;
}

/*
 * Check whether vec3 a and vec3 b are equal.
 * In case of equality, returns 1.
 * Otherwise, returns 0.
 */
int areEqualVec3(Vec3 a, Vec3 b)
{

    /* if x difference, y difference and z difference is smaller than threshold, then they are equal */
    if ((ABS((a.x - b.x)) < EPSILON) && (ABS((a.y - b.y)) < EPSILON) && (ABS((a.z - b.z)) < EPSILON))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * Returns an identity matrix (values on the diagonal are 1, others are 0).
 */
Matrix4 getIdentityMatrix()
{
    Matrix4 result;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                result.values[i][j] = 1.0;
            }
            else
            {
                result.values[i][j] = 0.0;
            }
        }
    }

    return result;
}

/*
 * Multiply matrices m1 (Matrix4) and m2 (Matrix4) and return the result matrix r (Matrix4).
 */
Matrix4 multiplyMatrixWithMatrix(Matrix4 m1, Matrix4 m2)
{
    Matrix4 result;
    double total;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            total = 0;
            for (int k = 0; k < 4; k++)
            {
                total += m1.values[i][k] * m2.values[k][j];
            }

            result.values[i][j] = total;
        }
    }

    return result;
}

/*
 * Multiply matrix m (Matrix4) with vector v (Vec4WithColor) and store the result in vector r (Vec4WithColor).
 */
Vec4WithColor multiplyMatrixWithVec4WithColor(Matrix4 m, Vec4WithColor v)
{
    double values[4];
    double total;

    for (int i = 0; i < 4; i++)
    {
        total = 0;
        for (int j = 0; j < 4; j++)
        {
            total += m.values[i][j] * v.getNthComponent(j);
        }
        values[i] = total;
    }

    return Vec4WithColor(values[0], values[1], values[2], values[3], v.color);
}



Matrix4 getTranslationMatrix(double tx, double ty, double tz) {
    Matrix4 result = getIdentityMatrix();
    result.values[0][3] = tx;
    result.values[1][3] = ty;
    result.values[2][3] = tz;
    return result;
}

Matrix4 getScalingMatrix(double sx, double sy, double sz) {
    Matrix4 result = getIdentityMatrix();
    result.values[0][0] = sx;
    result.values[1][1] = sy;
    result.values[2][2] = sz;
    return result;
}

Matrix4 getRotationMatrix(double angle, double ux, double uy, double uz) {
    Matrix4 result;
    Vec3 u = Vec3(ux, uy, uz);
    double magnitude = magnitudeOfVec3(u);
    if (magnitude < EPSILON) {
        return getIdentityMatrix(); // avoid division by zero
    }
    u = multiplyVec3WithScalar(u, 1.0 / magnitude);

    double radian = angle * PI / 180.0;
    double c = cos(radian);
    double s = sin(radian);
    double c_prime = 1.0 - c;

    result.values[0][0] = c + u.x * u.x * c_prime;
    result.values[0][1] = u.x * u.y * c_prime - u.z * s;
    result.values[0][2] = u.x * u.z * c_prime + u.y * s;
    result.values[0][3] = 0.0;

    result.values[1][0] = u.y * u.x * c_prime + u.z * s;
    result.values[1][1] = c + u.y * u.y * c_prime;
    result.values[1][2] = u.y * u.z * c_prime - u.x * s;
    result.values[1][3] = 0.0;

    result.values[2][0] = u.z * u.x * c_prime - u.y * s;
    result.values[2][1] = u.z * u.y * c_prime + u.x * s;
    result.values[2][2] = c + u.z * u.z * c_prime;
    result.values[2][3] = 0.0;

    result.values[3][0] = 0.0;
    result.values[3][1] = 0.0;
    result.values[3][2] = 0.0;
    result.values[3][3] = 1.0;

    return result;
}

Matrix4 getCameraTransformationMatrix(const Camera *camera) {
    Matrix4 T = getTranslationMatrix(-camera->position.x, -camera->position.y, -camera->position.z);
    
    Matrix4 R;
    R.values[0][0] = camera->u.x; 
    R.values[0][1] = camera->u.y; 
    R.values[0][2] = camera->u.z; 
    R.values[0][3] = 0.0;

    R.values[1][0] = camera->v.x; 
    R.values[1][1] = camera->v.y; 
    R.values[1][2] = camera->v.z; 
    R.values[1][3] = 0.0;

    R.values[2][0] = camera->w.x; 
    R.values[2][1] = camera->w.y; 
    R.values[2][2] = camera->w.z; 
    R.values[2][3] = 0.0;

    R.values[3][0] = 0.0;         
    R.values[3][1] = 0.0;         
    R.values[3][2] = 0.0;         
    R.values[3][3] = 1.0;
    
    return multiplyMatrixWithMatrix(R, T);
}

Matrix4 getOrthographicProjectionMatrix(const Camera *camera) {
    Matrix4 M_orth = getIdentityMatrix();
    double l = camera->left; 
    double r = camera->right;
    double b = camera->bottom; 
    double t = camera->top;
    double n = camera->near; 
    double f = camera->far;

    M_orth.values[0][0] = 2.0 / (r - l);
    M_orth.values[0][3] = -(r + l) / (r - l);
    M_orth.values[1][1] = 2.0 / (t - b);
    M_orth.values[1][3] = -(t + b) / (t - b);
    M_orth.values[2][2] = -2.0 / (f - n);
    M_orth.values[2][3] = -(f + n) / (f - n);
    
    return M_orth;
}

Matrix4 getPerspectiveProjectionMatrix(const Camera *camera) {
    Matrix4 M_per;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            M_per.values[i][j] = 0.0;
        }
    }
    
    double l = camera->left; 
    double r = camera->right;
    double b = camera->bottom; 
    double t = camera->top;
    double n = camera->near; 
    double f = camera->far;

    M_per.values[0][0] = 2.0 * n / (r - l);
    M_per.values[0][2] = (r + l) / (r - l);
    
    M_per.values[1][1] = 2.0 * n / (t - b);
    M_per.values[1][2] = (t + b) / (t - b);

    M_per.values[2][2] = -(f + n) / (f - n);
    M_per.values[2][3] = -2.0 * f * n / (f - n);
    
    M_per.values[3][2] = -1.0;
    
    return M_per;
}

Matrix4 getViewportTransformationMatrix(const Camera *camera) {
    Matrix4 M_vp;
    for(int i=0; i<3; i++) {
        for(int j=0; j<4; j++) {
            M_vp.values[i][j] = 0.0;
        }
    }

    double nx = (double)camera->horRes;
    double ny = (double)camera->verRes;

    M_vp.values[0][0] = nx / 2.0;
    M_vp.values[0][3] = (nx - 1.0) / 2.0;
    
    M_vp.values[1][1] = ny / 2.0;
    M_vp.values[1][3] = (ny - 1.0) / 2.0;
    
    M_vp.values[2][2] = 0.5;
    M_vp.values[2][3] = 0.5;
    
    return M_vp;
}

Vec3 calculateTriangleNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
    Vec3 edge1 = subtractVec3(v2, v1);
    Vec3 edge2 = subtractVec3(v3, v1);
    Vec3 n = crossProductVec3(edge1, edge2);
    return normalizeVec3(n);
}

bool isBackface(const Vec3& v1_cam, const Vec3& v2_cam, const Vec3& v3_cam) { // n.v > 0 can be culled where v is the vector from eye to any point on the triangle
    Vec3 n = calculateTriangleNormal(v1_cam, v2_cam, v3_cam);                 // here v is from eye to v1 --> since eye is at the origin, we simply used v1 coordinates
    if (dotProductVec3(n, v1_cam) > 0.0) {
        return true;
    }
    return false;
}

bool visible(double den, double num, double& t_E, double& t_L) {
    double t;
    if (den > EPSILON) { // potentially entering PE
        t = num / den;
        if (t > t_L) {
            return false;
        }
        else if (t > t_E) {
            t_E = t;
        }
    }
    else if (den < -EPSILON) { // potentially leaving PL
        t = num / den;
        if (t < t_E) {
            return false;
        }
        else if (t < t_L) {
            t_L = t;
        }
    }
    else if (num > 0.0) {
        return false;
    }

    return true;
}

bool clipLine(const Vec4WithColor& v1, const Vec4WithColor& v2, Vec4WithColor& v1_out, Vec4WithColor& v2_out)
{
    double t_E = 0.0;
    double t_L = 1.0;

    double dx = v2.x - v1.x;
    double dy = v2.y - v1.y;
    double dz = v2.z - v1.z;
    double dt = v2.t - v1.t;

    if (visible(dx + dt, -v1.x - v1.t, t_E, t_L) &&   // left: x >= -w
        visible(-dx + dt, v1.x - v1.t, t_E, t_L) &&   // right: x <= w
        visible(dy + dt, -v1.y - v1.t, t_E, t_L) &&   // bottom: y >= -w
        visible(-dy + dt, v1.y - v1.t, t_E, t_L) &&   // top: y <= w
        visible(dz + dt, -v1.z - v1.t, t_E, t_L) &&   // near: z >= -w
        visible(-dz + dt, v1.z - v1.t, t_E, t_L))     // far: z <= w    
    {
        v1_out = v1;
        v2_out = v2;

        if (t_E > 0.0)
        {
            double t = t_E;
            v1_out.x = v1.x + t * dx;
            v1_out.y = v1.y + t * dy;
            v1_out.z = v1.z + t * dz;
            v1_out.t = v1.t + t * dt;

            v1_out.color.r = v1.color.r + t * (v2.color.r - v1.color.r);
            v1_out.color.g = v1.color.g + t * (v2.color.g - v1.color.g);
            v1_out.color.b = v1.color.b + t * (v2.color.b - v1.color.b);
        }

        if (t_L < 1.0)
        {
            double t = t_L;
            v2_out.x = v1.x + t * dx;
            v2_out.y = v1.y + t * dy;
            v2_out.z = v1.z + t * dz;
            v2_out.t = v1.t + t * dt;

            v2_out.color.r = v1.color.r + t * (v2.color.r - v1.color.r);
            v2_out.color.g = v1.color.g + t * (v2.color.g - v1.color.g);
            v2_out.color.b = v1.color.b + t * (v2.color.b - v1.color.b);
        }
        return true;
    }
    return false;
}

void drawLine(int x0, int y0, double z0, Color c0, int x1, int y1, double z1, Color c1, Scene* scene, int horRes, int verRes) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    bool steep = ABS(dy) > ABS(dx);
    if (steep) { // if the line is steeper than 45 degree swap x and y values
        std::swap(x0, y0);
        std::swap(x1, y1);
        dx = x1 - x0;
        dy = y1 - y0;
    }

    if (x0 > x1) { // handle the case where the starting point is greater than the end point (x_0 > x_1) to ensure traversal is always x0 -> x1 (or y0 -> y1 if steep)
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(z0, z1);
        std::swap(c0, c1);
        dx = x1 - x0;
        dy = y1 - y0;
    }

    int step_y = (dy >= 0) ? 1 : -1;
    int abs_dy = ABS(dy);
    int abs_dx = ABS(dx);

    double total_steps = (double)abs_dx;
    if (total_steps <= 0.0) { // avoid division by zero
        total_steps = 1.0; 
    }

    double dz_step = (z1 - z0) / total_steps;
    double dr_step = (c1.r - c0.r) / total_steps;
    double dg_step = (c1.g - c0.g) / total_steps;
    double db_step = (c1.b - c0.b) / total_steps;

    double z_curr = z0;
    Color c_curr = c0;
    int y = y0;

    int d = 2 * abs_dy - abs_dx;
    int dE = 2 * abs_dy;
    int dNE = 2 * (abs_dy - abs_dx);

    for (int x = x0; x <= x1; x++) {
        int current_x = steep ? y : x;
        int current_y = steep ? x : y;

        if (current_x >= 0 && current_x < horRes && current_y >= 0 && current_y < verRes) {
            if (z_curr < scene->depth[current_x][current_y]) {
                scene->assignColorToPixel(current_x, current_y, c_curr);
                scene->depth[current_x][current_y] = z_curr;
            }
        }
        
        if (d < 0) {
            d += dE;
        } 
        else {
            d += dNE;
            y += step_y;
        }
        
        z_curr += dz_step;
        c_curr.r += dr_step;
        c_curr.g += dg_step;
        c_curr.b += db_step;
    }
}

double f_edge(double x, double y, const Vec4WithColor& v1, const Vec4WithColor& v2) {
    return (v1.y - v2.y) * x + (v2.x - v1.x) * y + v1.x * v2.y - v2.x * v1.y;
}

void rasterizeTriangle(const Vec4WithColor& v1_vp, const Vec4WithColor& v2_vp, const Vec4WithColor& v3_vp, Scene* scene, int horRes, int verRes) {
    double min_x = std::min({v1_vp.x, v2_vp.x, v3_vp.x});
    double max_x = std::max({v1_vp.x, v2_vp.x, v3_vp.x});
    double min_y = std::min({v1_vp.y, v2_vp.y, v3_vp.y});
    double max_y = std::max({v1_vp.y, v2_vp.y, v3_vp.y});

    int x_min = std::max(0, static_cast<int>(std::floor(min_x - 0.5)));
    int x_max = std::min(horRes - 1, static_cast<int>(std::ceil(max_x + 0.5)));
    int y_min = std::max(0, static_cast<int>(std::floor(min_y - 0.5)));
    int y_max = std::min(verRes - 1, static_cast<int>(std::ceil(max_y + 0.5)));

    double area_denom = f_edge(v3_vp.x, v3_vp.y, v1_vp, v2_vp);

    if (std::abs(area_denom) < EPSILON) {
        return;
    }

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            double px = x + 0.5;
            double py = y + 0.5;

            double alpha_num = f_edge(px, py, v2_vp, v3_vp);
            double beta_num = f_edge(px, py, v3_vp, v1_vp);
            double gamma_num = f_edge(px, py, v1_vp, v2_vp);

            double alpha = alpha_num / area_denom;
            double beta = beta_num / area_denom;
            double gamma = gamma_num / area_denom;

            if (alpha >= -EPSILON && beta >= -EPSILON && gamma >= -EPSILON) {
                double z_interp = alpha * v1_vp.z + beta * v2_vp.z + gamma * v3_vp.z;
                if (z_interp < scene->depth[x][y]) {
                    Color c_interp;
                    c_interp.r = alpha * v1_vp.color.r + beta * v2_vp.color.r + gamma * v3_vp.color.r;
                    c_interp.g = alpha * v1_vp.color.g + beta * v2_vp.color.g + gamma * v3_vp.color.g;
                    c_interp.b = alpha * v1_vp.color.b + beta * v2_vp.color.b + gamma * v3_vp.color.b;
                    scene->assignColorToPixel(x, y, c_interp);
                    scene->depth[x][y] = z_interp;
                }
            }
        }
    }
}
