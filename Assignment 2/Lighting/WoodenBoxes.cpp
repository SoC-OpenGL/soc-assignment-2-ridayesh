#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "Shader.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <stb-master/stb_image.h>
#include <vector>

using namespace glm;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(std::vector<std::string> faces);
GLuint BoxVAO, BoxVBO, LightCubeVAO, LightCubeVBO, SpaceVAO, SpaceVBO, texture, specularmap, normalmap, space_skybox;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
vec3 cameraPos = vec3(1.0f, 0.0f, -50.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, 1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
vec3 lightcolour = vec3(3.0f/255.0f, 161.0f/255.0f, 252.0f/255.0f);
vec3 lightambient = vec3(0.92f,0.95f,0.95f);
vec3 lightdiffuse = vec3(1.0f,1.0f,1.0f);
vec3 lightcube_pos = vec3(0.0f, 0.0f, -45.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float cutoff = -1;
const float sensitivity = 0.05f;
float yawX = 90.0f, pitchY = 0.0f;
float fov = 30.0f;
bool firstMouse = true,flashlight = false,BlinnPhong = false;
double lastX = 400.0f, lastY = 400.0f;
int main()
{
    float points[] = {
        -0.5f,0.5f,0.5f,0.0f,0.0f,0.0f,0.0f,1.0f,
        0.5f,0.5f,0.5f,1.0f,0.0f,0.0f,0.0f,1.0f,
        -0.5f,-0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,
        0.5f,-0.5f,0.5f,1.0f,1.0f,0.0f,0.0f,1.0f,
        -0.5f,-0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,
        0.5f,0.5f,0.5f,1.0f,0.0f,0.0f,0.0f,1.0f,
        -0.5f,0.5f,0.5f,1.0f,0.0f,0.0f,1.0f,0.0f,
        -0.5f,0.5f,-0.5f,1.0f,1.0f,0.0f,1.0f,0.0f,
        0.5f,0.5f,-0.5f,0.0f,1.0f,0.0f,1.0f,0.0f,
        -0.5f,0.5f,0.5f,1.0f,0.0f,0.0f,1.0f,0.0f,
        0.5f,0.5f,-0.5f,0.0f,1.0f,0.0f,1.0f,0.0f,
        0.5f,0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,0.0f,
        -0.5f,0.5f,0.5f,1.0f,1.0f,-1.0f,0.0f,0.0f,
        -0.5f,-0.5f,0.5f,1.0f,0.0f,-1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,0.0f,0.0f,-1.0f,0.0f,0.0f,
        -0.5f,0.5f,0.5f,1.0f,1.0f,-1.0f,0.0f,0.0f,
        -0.5f,0.5f,-0.5f,0.0f,1.0f,-1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,0.0f,0.0f,-1.0f,0.0f,0.0f,
        -0.5f,0.5f,-0.5f,0.0f,0.0f,0.0f,0.0f,-1.0f,
        0.5f,0.5f,-0.5f,1.0f,0.0f,0.0f,0.0f,-1.0f,
        -0.5f,-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,-1.0f,
        0.5f,-0.5f,-0.5f,1.0f,1.0f,0.0f,0.0f,-1.0f,
        -0.5f,-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,-1.0f,
        0.5f,0.5f,-0.5f,1.0f,0.0f,0.0f,0.0f,-1.0f,
        -0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f,-1.0f,0.0f,
        -0.5f,-0.5f,-0.5f,1.0f,1.0f,0.0f,-1.0f,0.0f,
        0.5f,-0.5f,-0.5f,0.0f,1.0f,0.0f,-1.0f,0.0f,
        -0.5f,-0.5f,0.5f,1.0f,0.0f,0.0f,-1.0f,0.0f,
        0.5f,-0.5f,-0.5f,0.0f,1.0f,0.0f,-1.0f,0.0f,
        0.5f,-0.5f,0.5f,0.0f,0.0f,0.0f,-1.0f,0.0f,
        0.5f,0.5f,0.5f,1.0f,1.0f,1.0f,0.0f,0.0f,
        0.5f,-0.5f,0.5f,1.0f,0.0f,1.0f,0.0f,0.0f,
        0.5f,-0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,0.0f,
        0.5f,0.5f,0.5f,1.0f,1.0f,1.0f,0.0f,0.0f,
        0.5f,0.5f,-0.5f,0.0f,1.0f,1.0f,0.0f,0.0f,
        0.5f,-0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,0.0f

    };
    float LightCube[] = {
        -0.5f,0.5f,0.5f,
        0.5f,0.5f,0.5f,
        -0.5f,-0.5f,0.5f,
        0.5f,-0.5f,0.5f,
        -0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,
        -0.5f,0.5f,0.5f,
        -0.5f,0.5f,-0.5f,
        0.5f,0.5f,-0.5f,
        -0.5f,0.5f,0.5f,
        0.5f,0.5f,-0.5f,
        0.5f,0.5f,0.5f,
        -0.5f,0.5f,0.5f,
        -0.5f,-0.5f,0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,0.5f,0.5f,
        -0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,0.5f,-0.5f,
        0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,
        0.5f,-0.5f,0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,0.5f,0.5f,
        0.5f,0.5f,-0.5f,
        0.5f,-0.5f,-0.5f

    };
    float background_space[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    vec3 positions[] = {
        vec3(5.0f,4.0f,7.0f),
        vec3(0.0f,0.0f,10.0f),
        vec3(-2.0f,3.0f,2.5f),
        vec3(4.5f,-4.0f,-4.0f),
        vec3(0.0f,0.0f,-10.0f)
    };
    float rotations[] = {
        60.0f,30.0f,12.0f,78.0f,90.0f
    };
    vec3 axis[] = {
        vec3(1.0f,1.0f,1.0f),
        vec3(0.5f,0.7f,0.1f),
        vec3(0.9f,0.0f,0.0f),
        vec3(0.6f,-0.5f,-0.5f),
        vec3(0.0f,0.0f,1.0f)
    };
    std::vector <std::string> background = {
        "C:/Users/Ramesh/Desktop/bkg/blue/right.png",
        "C:/Users/Ramesh/Desktop/bkg/blue/left.png",
        "C:/Users/Ramesh/Desktop/bkg/blue/top.png",
        "C:/Users/Ramesh/Desktop/bkg/blue/bot.png",
        "C:/Users/Ramesh/Desktop/bkg/blue/front.png",
        "C:/Users/Ramesh/Desktop/bkg/blue/back.png"
    };
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "First Success", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "FAILED TO CREATE WINDOW";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
   // glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "FAILED TO INITIALIZE GLAD";
    }
    glEnable(GL_DEPTH_TEST);
    glGenBuffers(1, &BoxVBO);
    glGenVertexArrays(1, &BoxVAO);
    glBindVertexArray(BoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, BoxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glGenBuffers(1, &LightCubeVBO);
    glGenVertexArrays(1, &LightCubeVAO);
    glBindVertexArray(LightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, LightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LightCube), LightCube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenBuffers(1, &SpaceVBO);
    glGenVertexArrays(1, &SpaceVAO);
    glBindVertexArray(SpaceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, SpaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(background_space), background_space, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    texture = loadTexture("C:/Users/Ramesh/Desktop/WoodenBox.png");
    specularmap = loadTexture("C:/Users/Ramesh/Desktop/WoodenSpecularMap.png");
    normalmap = loadTexture("C:/Users/Ramesh/Desktop/NormalMap.png");
    space_skybox = loadCubemap(background);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularmap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalmap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, space_skybox);

    Shader Cube("C:/Users/Ramesh/Desktop/Shaders/WoodenBoxVs.txt", "C:/Users/Ramesh/Desktop/Shaders/WoodenBoxFs.txt");
    Shader LightSource("C:/Users/Ramesh/Desktop/Shaders/LightCubeVs.txt", "C:/Users/Ramesh/Desktop/Shaders/LightCubeFs.txt");
    Shader skybox("C:/Users/Ramesh/Desktop/Shaders/SpaceSkyboxVs.txt","C:/Users/Ramesh/Desktop/Shaders/SpaceSkyboxFs.txt");
    Cube.use();
    Cube.setInt("box.box_patch", 0);
    Cube.setInt("box.spec_patch", 1);
    Cube.setInt("box.normal_patch", 2);
    mat4 view,projection,model = mat4(1.0f);
    mat3 transform_normal;
    LightSource.use();
    LightSource.setVec3("lightcolour", lightcolour);
    skybox.use();
    skybox.setMat4("model", model);
    //skybox.setInt("space", 3);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthMask(GL_FALSE);
        view = mat4(mat3(lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
        model = mat4(1.0f);
        projection = perspective(radians(fov), (float)SCR_HEIGHT / (float)SCR_WIDTH, 0.1f, 500.0f);
        skybox.use();
        skybox.setMat4("model", model);
        skybox.setMat4("view", view);
        skybox.setMat4("projection", projection);
        glBindVertexArray(SpaceVAO);        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        Cube.use();
        Cube.setFloat("box.ambientfactor", 0.4f);
        Cube.setInt("box.shineness", 100);
        Cube.setMat4("view", view);
        Cube.setMat4("projection", projection);
        Cube.setVec3("light.colour", lightcolour);
        Cube.setVec3("light.diffuse",lightdiffuse);
        Cube.setVec3("light.ambient", lightambient);
        if (!flashlight)
        {
            Cube.setVec3("light.position", lightcube_pos);
            Cube.setFloat("light.cutoff", cutoff);
        }
        else
        {
            Cube.setVec3("light.position", cameraPos);
            Cube.setFloat("light.cutoff", cutoff);
            Cube.setVec3("light.FlashlightDir", cameraFront);
        }
        Cube.setVec3("playerpos", cameraPos);
        Cube.setFloat("light.constant", 1.0f);
        Cube.setFloat("light.linear", 0.027f);
        Cube.setFloat("light.quadratic", 0.00028f);
        if (BlinnPhong)
            Cube.setInt("BlinnPhong", 1);
        else
            Cube.setInt("BlinnPhong", 0);
        for (int i = 0;i <= 4;i++)
        {
            model = mat4(1.0f);
            model = scale(model, vec3(1.5f, 1.5f, 1.5f));
            model = translate(model, positions[i]);
            model = rotate(model, rotations[i], axis[i]);
            Cube.setMat4("model", model);
            transform_normal = mat3(transpose(inverse(model)));
            Cube.setMat3("transform_normal", transform_normal);
            glBindVertexArray(BoxVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        model = mat4(1.0f);
        model = scale(model, vec3(0.5f, 0.5f, 0.5f));
        model = translate(model, lightcube_pos);
        LightSource.use();
        LightSource.setMat4("view", view);
        LightSource.setMat4("projection", projection);
        LightSource.setMat4("model", model);
        glBindVertexArray(LightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    float cameraSpeed = 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos += cameraSpeed * normalize(cross(cameraUp, cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos -= cameraSpeed * normalize(cross(cameraUp, cameraFront));
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    if (fov > 10.0f && fov < 45.0f)
    {
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            fov -= deltaTime*10.0f;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            fov += deltaTime*10.0f;
    }
    if (fov >= 45.0f)
    {
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            fov = 45.0f;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            fov -= 1.0f;

    }
    if (fov <= 10.0f)
    {
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            fov += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            fov = 10.0f;

    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        cutoff = cos(radians(7.0f));
        flashlight = true;
        glBindVertexArray(LightCubeVAO);
        glDisableVertexAttribArray(0);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        cutoff = -1;
        flashlight = false;
        glBindVertexArray(LightCubeVAO);
        glEnableVertexAttribArray(0);

    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        BlinnPhong = true;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        BlinnPhong = false;
    }
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yawX += xoffset;
    pitchY += yoffset;
    if (pitchY > 89.0f)
        pitchY = 89.0f;
    if (pitchY < -89.0f)
        pitchY = -89.0f;
    vec3 direction;
    direction.x = cos(radians(yawX)) * cos(radians(pitchY));
    direction.y = sin(radians(pitchY));
    direction.z = cos(radians(pitchY)) * sin(radians(yawX));
    cameraFront = normalize(direction);

}
/*void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

    if (fov >= 1.0f && fov <= 45.0f)
        fov -= 1.0f;
        // fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}
       THIS DOES NOT DISTINGUISH BETWEEN A ZOOM IN AND A ZOOM OUT..HENCE USED KEYBOARD KEYS AND NOT SCROLL.
*/

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

