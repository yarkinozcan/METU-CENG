#include <cstdio>
#include <array>
#include <cmath>

#include "utility.h"

#include <GLFW/glfw3.h>

#include <glm/ext.hpp> // for matrix calculation

struct PlanetData
{
    glm::vec3 position;
    float rotation;
    float orbitRadius;
    float orbitSpeed;
    float rotationSpeed;
    float size;
    bool isEarth;
    int parentIndex;
};

struct CameraState
{
    glm::vec3 pos;
    glm::vec3 gaze;
    glm::vec3 up;
    float distance;
    float fov;
    int targetPlanet;
};

GLState* g_state = nullptr;
CameraState g_cam;
std::array<PlanetData, 3> g_planets;
float g_timeScale = 1.0f;
float g_time = 0.0f;
bool g_mouseDown = false;
double g_lastX = 0.0;
double g_lastY = 0.0;
float g_yaw = 0.0f;
float g_pitch = 0.0f;

void UpdateCamera()
{
    if(g_cam.targetPlanet == -1)
    {
        // FPS mode
        glm::vec3 fwd = glm::vec3(
            cos(g_pitch) * sin(g_yaw),
            sin(g_pitch),
            cos(g_pitch) * cos(g_yaw)
        );
        glm::vec3 r = glm::normalize(glm::cross(fwd, glm::vec3(0, 1, 0)));
        glm::vec3 u = glm::cross(r, fwd);
        
        g_cam.gaze = g_cam.pos + fwd;
        g_cam.up = u;
    }
    else
    {
        // Orbit mode
        const PlanetData& p = g_planets[g_cam.targetPlanet];
        g_cam.gaze = p.position;
        g_cam.pos = p.position + glm::vec3(
            g_cam.distance * cos(g_pitch) * sin(g_yaw),
            g_cam.distance * sin(g_pitch),
            g_cam.distance * cos(g_pitch) * cos(g_yaw)
        );
    }
}

void MouseMoveCallback(GLFWwindow* wnd, double x, double y)
{
    if(!g_mouseDown) return;
    
    double dx = x - g_lastX;
    double dy = y - g_lastY;
    
    g_yaw -= float(dx) * 0.01f;
    
    // FPS mode: standard controls (up = look up)
    // Orbit mode: inverted drag-style
    if(g_cam.targetPlanet == -1)
        g_pitch -= float(dy) * 0.01f;
    else
        g_pitch += float(dy) * 0.01f;
    
    g_pitch = glm::clamp(g_pitch, -glm::pi<float>() * 0.5f + 0.1f, glm::pi<float>() * 0.5f - 0.1f);
    
    UpdateCamera();
    
    g_lastX = x;
    g_lastY = y;
}

void MouseButtonCallback(GLFWwindow* wnd, int button, int action, int)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        g_mouseDown = (action == GLFW_PRESS);
        if(g_mouseDown)
            glfwGetCursorPos(wnd, &g_lastX, &g_lastY);
    }
}

void MouseScrollCallback(GLFWwindow* wnd, double dx, double dy)
{
    if(g_cam.targetPlanet == -1)
    {
        // FPS zoom via FOV
        g_cam.fov -= float(dy) * 2.0f;
        g_cam.fov = glm::clamp(g_cam.fov, 20.0f, 90.0f);
    }
    else
    {
        // Orbit zoom via distance
        g_cam.distance *= (1.0f - float(dy) * 0.1f);
        g_cam.distance = glm::clamp(g_cam.distance, 0.5f, 50.0f);
        UpdateCamera();
    }
}

void FramebufferChangeCallback(GLFWwindow* wnd, int w, int h)
{
    GLState* state = static_cast<GLState*>(glfwGetWindowUserPointer(wnd));
    state->width = w;
    state->height = h;
}

void KeyboardCallback(GLFWwindow* wnd, int key, int scancode, int action, int modifier)
{
    GLState* state = static_cast<GLState*>(glfwGetWindowUserPointer(wnd));
    uint32_t mode = state->mode;

    if(action == GLFW_RELEASE)
    {
        if(key == GLFW_KEY_P)
        {
            int prev = mode;
            mode = (mode == 3) ? 0 : (mode + 1);
            g_cam.targetPlanet = (mode == 3) ? -1 : int(mode);
            
            // Switching to FPS - calc yaw/pitch from current view
            if(g_cam.targetPlanet == -1 && prev != 3)
            {
                glm::vec3 dir = glm::normalize(g_cam.gaze - g_cam.pos);
                g_pitch = float(asin(dir.y));
                g_yaw = float(atan2(dir.x, dir.z));
            }
            else if(g_cam.targetPlanet != -1)
            {
                g_yaw = 0.0f;
                g_pitch = 0.3f;
                g_cam.distance = 5.0f;
            }
            UpdateCamera();
        }
        if(key == GLFW_KEY_O)
        {
            int prev = mode;
            mode = (mode == 0) ? 3 : (mode - 1);
            g_cam.targetPlanet = (mode == 3) ? -1 : int(mode);
            
            if(g_cam.targetPlanet == -1 && prev != 3)
            {
                glm::vec3 dir = glm::normalize(g_cam.gaze - g_cam.pos);
                g_pitch = float(asin(dir.y));
                g_yaw = float(atan2(dir.x, dir.z));
            }
            else if(g_cam.targetPlanet != -1)
            {
                g_yaw = 0.0f;
                g_pitch = 0.3f;
                g_cam.distance = 5.0f;
            }
            UpdateCamera();
        }
        if(key == GLFW_KEY_L)
        {
            g_timeScale += 0.5f;
            if(g_timeScale > 10.0f) g_timeScale = -10.0f;
        }
        if(key == GLFW_KEY_K)
        {
            g_timeScale -= 0.5f;
            if(g_timeScale < -10.0f) g_timeScale = 10.0f;
        }
        
        state->mode = mode;
    }
    
    // WASD movement in FPS mode
    if(g_cam.targetPlanet == -1 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        glm::vec3 fwd = glm::normalize(g_cam.gaze - g_cam.pos);
        glm::vec3 right = glm::normalize(glm::cross(fwd, g_cam.up));
        float spd = 0.08f;
        
        if(key == GLFW_KEY_W) { g_cam.pos += fwd * spd; g_cam.gaze += fwd * spd; }
        if(key == GLFW_KEY_S) { g_cam.pos -= fwd * spd; g_cam.gaze -= fwd * spd; }
        if(key == GLFW_KEY_A) { g_cam.pos -= right * spd; g_cam.gaze -= right * spd; }
        if(key == GLFW_KEY_D) { g_cam.pos += right * spd; g_cam.gaze += right * spd; }
    }
}

int main(int argc, const char* argv[])
{
    GLState state = GLState("Planet Renderer", 1280, 720,
                            CallbackPointersGLFW());
    g_state = &state;
    state.mode = 0; // Start in orbit mode
    
    // =============== //
    //  PLANET SETUP   //
    // =============== //
    g_planets[0] = {glm::vec3(0), 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, true, -1};      // Earth
    g_planets[1] = {glm::vec3(3, 0, 0), 0.0f, 3.0f, 1.0f, 0.3f, 0.3f, false, 0}; // Moon
    g_planets[2] = {glm::vec3(0.5, 0, 0), 0.0f, 0.5f, 2.0f, 0.5f, 0.1f, false, 1}; // Moon's moon
    
    // Camera init
    g_cam.pos = glm::vec3(0, 2, 8);
    g_cam.gaze = glm::vec3(0, 0, 0);
    g_cam.up = glm::vec3(0, 1, 0);
    g_cam.distance = 5.0f;
    g_cam.fov = 50.0f;
    g_cam.targetPlanet = 0;
    g_yaw = 0.0f;
    g_pitch = 0.3f;
    UpdateCamera();
    
    // =============== //
    //  LOAD SHADERS   //
    // =============== //
    ShaderGL planetVert = ShaderGL(ShaderGL::VERTEX, "shaders/planet.vert");
    ShaderGL planetFrag = ShaderGL(ShaderGL::FRAGMENT, "shaders/planet.frag");
    ShaderGL shadowVert = ShaderGL(ShaderGL::VERTEX, "shaders/shadow.vert");
    ShaderGL shadowFrag = ShaderGL(ShaderGL::FRAGMENT, "shaders/shadow.frag");
    
    // =============== //
    //   LOAD MESH     //
    // =============== //
    MeshGL sphereMesh = MeshGL("meshes/sphere_20k.obj");
    
    // =============== //
    //  LOAD TEXTURES  //
    // =============== //
    TextureGL earthTex = TextureGL("textures/2k_earth_daymap.jpg",
                                   TextureGL::LINEAR, TextureGL::REPEAT);
    TextureGL earthSpec = TextureGL("textures/2k_earth_specular_map.png",
                                    TextureGL::LINEAR, TextureGL::REPEAT);
    TextureGL earthNight = TextureGL("textures/2k_earth_nightmap_alpha.png",
                                     TextureGL::LINEAR, TextureGL::REPEAT);
    TextureGL cloudsTex = TextureGL("textures/2k_earth_clouds_alpha.png",
                                    TextureGL::LINEAR, TextureGL::REPEAT);
    TextureGL moonTex = TextureGL("textures/2k_moon.jpg",
                                  TextureGL::LINEAR, TextureGL::REPEAT);
    TextureGL jupiterTex = TextureGL("textures/2k_jupiter.jpg",
                                     TextureGL::LINEAR, TextureGL::REPEAT);
    TextureGL starsTex = TextureGL("textures/8k_stars_milky_way.jpg",
                                   TextureGL::LINEAR, TextureGL::REPEAT);
    
    // =============== //
    //   SHADOW FBO    //
    // =============== //
    GLuint shadowFBO = 0;
    GLuint shadowDepth = 0;
    GLuint shadowColor = 0;
    int shadowRes = 4096;
    
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    
    // Depth texture
    glGenTextures(1, &shadowDepth);
    glBindTexture(GL_TEXTURE_2D, shadowDepth);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, shadowRes, shadowRes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepth, 0);
    
    // Color texture for depth value
    glGenTextures(1, &shadowColor);
    glBindTexture(GL_TEXTURE_2D, shadowColor);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, shadowRes, shadowRes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowColor, 0);
    
    GLenum bufs[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, bufs);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::fprintf(stderr, "Shadow FBO incomplete!\n");
        std::exit(1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // =============== //
    // UNIFORM LOCS    //
    // =============== //
    // These must match the uniform "location" at the shader(s).
    // Vertex shader uniforms
    static constexpr GLuint U_TRANSFORM_MODEL  = 0;
    static constexpr GLuint U_TRANSFORM_VIEW   = 1;
    static constexpr GLuint U_TRANSFORM_PROJ   = 2;
    static constexpr GLuint U_TRANSFORM_NORMAL = 3;
    static constexpr GLuint U_LIGHT_VP         = 4;
    // Fragment shader uniforms
    static constexpr GLuint U_LIGHT_DIR   = 0;
    static constexpr GLuint U_LIGHT_COLOR = 1;
    static constexpr GLuint U_CAMERA_POS  = 2;
    static constexpr GLuint U_AMBIENT      = 3;
    static constexpr GLuint U_SHADOW_BIAS = 4;
    static constexpr GLuint U_IS_EARTH    = 5;
    static constexpr GLuint U_IS_BACKGROUND = 6;
    static constexpr GLuint U_IS_SUN      = 7;
    static constexpr GLuint U_IS_CLOUD    = 8;
    // Texture bindings
    static constexpr GLuint T_ALBEDO   = 0;
    static constexpr GLuint T_SPECULAR = 1;
    static constexpr GLuint T_NIGHT    = 2;
    static constexpr GLuint T_SHADOW   = 3;
    
    // Set unchanged state(s)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    float lastTime = float(glfwGetTime());
    
    // =============== //
    //   RENDER LOOP   //
    // =============== //
    while(!glfwWindowShouldClose(state.window))
    {
        // Poll inputs from the OS via GLFW
        glfwPollEvents();
        
        // Time update
        float currentTime = float(glfwGetTime());
        float dt = (currentTime - lastTime) * g_timeScale;
        g_time += dt;
        lastTime = currentTime;
        
        // Update planet positions
        for(int i = 0; i < 3; i++)
        {
            PlanetData& pl = g_planets[i];
            pl.rotation += pl.rotationSpeed * dt;
            
            if(pl.parentIndex >= 0)
            {
                const PlanetData& parent = g_planets[pl.parentIndex];
                float ang = pl.orbitSpeed * g_time;
                pl.position = parent.position + glm::vec3(
                    pl.orbitRadius * cos(ang),
                    0.0f,
                    pl.orbitRadius * sin(ang)
                );
            }
        }
        
        UpdateCamera();
        
        // Light setup (rotating sun)
        float lightAng = g_time * 0.3f;
        glm::vec3 lightDir = glm::normalize(glm::vec3(cos(lightAng), 0.0f, sin(lightAng)));
        glm::vec3 lightColor = glm::vec3(0.8f, 0.75f, 0.65f);
        
        // Light matrices for shadow mapping
        glm::vec3 lightPos = -lightDir * 30.0f;
        glm::mat4x4 lightView = glm::lookAt(lightPos, glm::vec3(0), glm::vec3(0, 1, 0));
        glm::mat4x4 lightProj = glm::ortho(-8.0f, 8.0f, -8.0f, 8.0f, 1.0f, 60.0f);
        glm::mat4x4 lightVP = lightProj * lightView;
        
        // =============== //
        //  SHADOW PASS    //
        // =============== //
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glViewport(0, 0, shadowRes, shadowRes);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        
        glUseProgramStages(state.renderPipeline, GL_VERTEX_SHADER_BIT, shadowVert.shaderId);
        glUseProgramStages(state.renderPipeline, GL_FRAGMENT_SHADER_BIT, shadowFrag.shaderId);
        
        // glActiveShaderProgram makes "glUniform" family of commands
        // to effect the selected shader
        glActiveShaderProgram(state.renderPipeline, shadowVert.shaderId);
        glUniformMatrix4fv(U_TRANSFORM_VIEW, 1, false, glm::value_ptr(lightView));
        glUniformMatrix4fv(U_TRANSFORM_PROJ, 1, false, glm::value_ptr(lightProj));
        
        glBindVertexArray(sphereMesh.vaoId);
        
        for(int i = 0; i < 3; i++)
        {
            glm::mat4x4 model = glm::translate(glm::mat4x4(1.0f), g_planets[i].position);
            model = glm::rotate(model, g_planets[i].rotation, glm::vec3(0, 1, 0));
            model = glm::scale(model, glm::vec3(g_planets[i].size));
            
            glUniformMatrix4fv(U_TRANSFORM_MODEL, 1, false, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, sphereMesh.indexCount, GL_UNSIGNED_INT, nullptr);
        }
        
        // =============== //
        //   MAIN PASS     //
        // =============== //
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, state.width, state.height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Object-common matrices
        glm::mat4x4 proj = glm::perspective(glm::radians(g_cam.fov),
                                            float(state.width) / float(state.height),
                                            0.01f, 100.0f);
        glm::mat4x4 view = glm::lookAt(g_cam.pos, g_cam.gaze, g_cam.up);
        
        // =============== //
        //   BACKGROUND    //
        // =============== //
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        
        glUseProgramStages(state.renderPipeline, GL_VERTEX_SHADER_BIT, planetVert.shaderId);
        glUseProgramStages(state.renderPipeline, GL_FRAGMENT_SHADER_BIT, planetFrag.shaderId);
        
        glActiveShaderProgram(state.renderPipeline, planetVert.shaderId);
        {
            glm::mat4x4 model = glm::translate(glm::mat4x4(1.0f), g_cam.pos);
            model = glm::scale(model, glm::vec3(90.0f));
            glUniformMatrix4fv(U_TRANSFORM_MODEL, 1, false, glm::value_ptr(model));
            glUniformMatrix4fv(U_TRANSFORM_VIEW, 1, false, glm::value_ptr(view));
            glUniformMatrix4fv(U_TRANSFORM_PROJ, 1, false, glm::value_ptr(proj));
            // Dummy values for unused uniforms (background doesn't need them)
            glm::mat3x3 dummyNormal = glm::mat3x3(1.0f);
            glm::mat4x4 dummyLightVP = glm::mat4x4(1.0f);
            glUniformMatrix3fv(U_TRANSFORM_NORMAL, 1, false, glm::value_ptr(dummyNormal));
            glUniformMatrix4fv(U_LIGHT_VP, 1, false, glm::value_ptr(dummyLightVP));
        }
        
        glActiveShaderProgram(state.renderPipeline, planetFrag.shaderId);
        {
            glUniform1f(U_IS_BACKGROUND, 1.0f);
            glUniform1f(U_IS_SUN, 0.0f);
            glUniform1f(U_IS_CLOUD, 0.0f);
            glUniform1f(U_IS_EARTH, 0.0f);
        }
        
        // Bind texture
        glActiveTexture(GL_TEXTURE0 + T_ALBEDO);
        glBindTexture(GL_TEXTURE_2D, starsTex.textureId);
        
        glBindVertexArray(sphereMesh.vaoId);
        glDrawElements(GL_TRIANGLES, sphereMesh.indexCount, GL_UNSIGNED_INT, nullptr);
        
        // =============== //
        //      SUN        //
        // =============== //
        glUseProgramStages(state.renderPipeline, GL_VERTEX_SHADER_BIT, planetVert.shaderId);
        glUseProgramStages(state.renderPipeline, GL_FRAGMENT_SHADER_BIT, planetFrag.shaderId);
        
        glActiveShaderProgram(state.renderPipeline, planetVert.shaderId);
        {
            glm::vec3 sunPos = g_cam.pos - lightDir * 85.0f;
            glm::mat4x4 model = glm::translate(glm::mat4x4(1.0f), sunPos);
            model = glm::scale(model, glm::vec3(0.8f));
            glUniformMatrix4fv(U_TRANSFORM_MODEL, 1, false, glm::value_ptr(model));
            glUniformMatrix4fv(U_TRANSFORM_VIEW, 1, false, glm::value_ptr(view));
            glUniformMatrix4fv(U_TRANSFORM_PROJ, 1, false, glm::value_ptr(proj));
            // Dummy values for unused uniforms (sun doesn't need them)
            glm::mat3x3 dummyNormal = glm::mat3x3(1.0f);
            glm::mat4x4 dummyLightVP = glm::mat4x4(1.0f);
            glUniformMatrix3fv(U_TRANSFORM_NORMAL, 1, false, glm::value_ptr(dummyNormal));
            glUniformMatrix4fv(U_LIGHT_VP, 1, false, glm::value_ptr(dummyLightVP));
        }
        glActiveShaderProgram(state.renderPipeline, planetFrag.shaderId);
        {
            glUniform3fv(U_LIGHT_COLOR, 1, glm::value_ptr(lightColor));
            glUniform1f(U_IS_BACKGROUND, 0.0f);
            glUniform1f(U_IS_SUN, 1.0f);
            glUniform1f(U_IS_CLOUD, 0.0f);
            glUniform1f(U_IS_EARTH, 0.0f);
        }
        
        glBindVertexArray(sphereMesh.vaoId);
        glDrawElements(GL_TRIANGLES, sphereMesh.indexCount, GL_UNSIGNED_INT, nullptr);
        
        // =============== //
        //    PLANETS      //
        // =============== //
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        
        glUseProgramStages(state.renderPipeline, GL_VERTEX_SHADER_BIT, planetVert.shaderId);
        
        glActiveShaderProgram(state.renderPipeline, planetVert.shaderId);
        glUniformMatrix4fv(U_TRANSFORM_VIEW, 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(U_TRANSFORM_PROJ, 1, false, glm::value_ptr(proj));
        glUniformMatrix4fv(U_LIGHT_VP, 1, false, glm::value_ptr(lightVP));
        
        glBindVertexArray(sphereMesh.vaoId);
        
        for(int i = 0; i < 3; i++)
        {
            const PlanetData& pl = g_planets[i];
            
            glm::mat4x4 model = glm::translate(glm::mat4x4(1.0f), pl.position);
            model = glm::rotate(model, pl.rotation, glm::vec3(0, 1, 0));
            model = glm::scale(model, glm::vec3(pl.size));
            glm::mat3x3 normalMatrix = glm::inverseTranspose(glm::mat3x3(model));
            
            glActiveShaderProgram(state.renderPipeline, planetVert.shaderId);
            glUniformMatrix4fv(U_TRANSFORM_MODEL, 1, false, glm::value_ptr(model));
            glUniformMatrix3fv(U_TRANSFORM_NORMAL, 1, false, glm::value_ptr(normalMatrix));
            
            if(pl.isEarth)
            {
                // Render Earth with planet shader (Earth mode enabled)
                glUseProgramStages(state.renderPipeline, GL_FRAGMENT_SHADER_BIT, planetFrag.shaderId);
                glActiveShaderProgram(state.renderPipeline, planetFrag.shaderId);
                {
                    glUniform3fv(U_LIGHT_DIR, 1, glm::value_ptr(lightDir));
                    glUniform3fv(U_LIGHT_COLOR, 1, glm::value_ptr(lightColor));
                    glUniform3fv(U_CAMERA_POS, 1, glm::value_ptr(g_cam.pos));
                    glUniform1f(U_AMBIENT, 0.01f);
                    glUniform1f(U_SHADOW_BIAS, 0.002f);
                    glUniform1f(U_IS_BACKGROUND, 0.0f);
                    glUniform1f(U_IS_SUN, 0.0f);
                    glUniform1f(U_IS_CLOUD, 0.0f);
                    glUniform1f(U_IS_EARTH, 1.0f);
                }
                
                // Bind textures
                glActiveTexture(GL_TEXTURE0 + T_ALBEDO);
                glBindTexture(GL_TEXTURE_2D, earthTex.textureId);
                glActiveTexture(GL_TEXTURE0 + T_SPECULAR);
                glBindTexture(GL_TEXTURE_2D, earthSpec.textureId);
                glActiveTexture(GL_TEXTURE0 + T_NIGHT);
                glBindTexture(GL_TEXTURE_2D, earthNight.textureId);
                glActiveTexture(GL_TEXTURE0 + T_SHADOW);
                glBindTexture(GL_TEXTURE_2D, shadowColor);
                
                glDrawElements(GL_TRIANGLES, sphereMesh.indexCount, GL_UNSIGNED_INT, nullptr);
                
                // Render clouds with alpha blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                
                float cloudRot = g_time * 0.3f;
                glm::mat4x4 cloudModel = glm::translate(glm::mat4x4(1.0f), pl.position);
                cloudModel = glm::rotate(cloudModel, cloudRot, glm::vec3(0.0f, 1.0f, 0.0f));
                cloudModel = glm::scale(cloudModel, glm::vec3(pl.size * 1.01f));
                glm::mat3x3 cloudNormal = glm::inverseTranspose(glm::mat3x3(cloudModel));
                
                glUseProgramStages(state.renderPipeline, GL_FRAGMENT_SHADER_BIT, planetFrag.shaderId);
                
                glActiveShaderProgram(state.renderPipeline, planetVert.shaderId);
                glUniformMatrix4fv(U_TRANSFORM_MODEL, 1, false, glm::value_ptr(cloudModel));
                glUniformMatrix3fv(U_TRANSFORM_NORMAL, 1, false, glm::value_ptr(cloudNormal));
                
                glActiveShaderProgram(state.renderPipeline, planetFrag.shaderId);
                {
                    glUniform3fv(U_LIGHT_DIR, 1, glm::value_ptr(lightDir));
                    glUniform3fv(U_LIGHT_COLOR, 1, glm::value_ptr(lightColor));
                    glUniform1f(U_IS_BACKGROUND, 0.0f);
                    glUniform1f(U_IS_SUN, 0.0f);
                    glUniform1f(U_IS_CLOUD, 1.0f);
                    glUniform1f(U_IS_EARTH, 0.0f);
                }
                
                glActiveTexture(GL_TEXTURE0 + T_ALBEDO);
                glBindTexture(GL_TEXTURE_2D, cloudsTex.textureId);
                
                glDrawElements(GL_TRIANGLES, sphereMesh.indexCount, GL_UNSIGNED_INT, nullptr);
                glDisable(GL_BLEND);
            }
            else
            {
                // Render regular planet (moon)
                glUseProgramStages(state.renderPipeline, GL_FRAGMENT_SHADER_BIT, planetFrag.shaderId);
                glActiveShaderProgram(state.renderPipeline, planetFrag.shaderId);
                {
                    glUniform3fv(U_LIGHT_DIR, 1, glm::value_ptr(lightDir));
                    glUniform3fv(U_LIGHT_COLOR, 1, glm::value_ptr(lightColor));
                    glUniform3fv(U_CAMERA_POS, 1, glm::value_ptr(g_cam.pos));
                    glUniform1f(U_AMBIENT, 0.01f);
                    glUniform1f(U_SHADOW_BIAS, 0.002f);
                    glUniform1f(U_IS_BACKGROUND, 0.0f);
                    glUniform1f(U_IS_SUN, 0.0f);
                    glUniform1f(U_IS_CLOUD, 0.0f);
                    glUniform1f(U_IS_EARTH, 0.0f);
                }
                
                // Bind textures
                glActiveTexture(GL_TEXTURE0 + T_ALBEDO);
                if(i == 2)
                    glBindTexture(GL_TEXTURE_2D, jupiterTex.textureId);
                else
                    glBindTexture(GL_TEXTURE_2D, moonTex.textureId);
                glActiveTexture(GL_TEXTURE0 + T_SPECULAR);
                glBindTexture(GL_TEXTURE_2D, moonTex.textureId);
                glActiveTexture(GL_TEXTURE0 + T_NIGHT);
                glBindTexture(GL_TEXTURE_2D, moonTex.textureId);
                glActiveTexture(GL_TEXTURE0 + T_SHADOW);
                glBindTexture(GL_TEXTURE_2D, shadowColor);
                
                glDrawElements(GL_TRIANGLES, sphereMesh.indexCount, GL_UNSIGNED_INT, nullptr);
            }
        }
        
        glfwSwapBuffers(state.window);
    }
    
    // Cleanup
    glDeleteFramebuffers(1, &shadowFBO);
    glDeleteTextures(1, &shadowDepth);
    glDeleteTextures(1, &shadowColor);
    
    return 0;
}
