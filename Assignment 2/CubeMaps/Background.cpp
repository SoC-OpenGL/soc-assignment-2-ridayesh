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
unsigned int loadCubemap(std::vector<std::string> faces);
GLuint backTexture, vaoB, vboB, vbo, vao, texture;
int height, width, ColourChannels;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
vec3 cameraPos = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, 1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
const float sensitivity = 0.05f;
float yawX = 90.0f, pitchY = 0.0f;
float fov = 30.0f;
bool firstMouse = true;
double lastX = 400.0f, lastY = 400.0f;
int main()
{
    float background[] = {
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
    float points[] = {
        -0.5f,0.5f,0.5f,0.0f,0.0f,
        0.5f,0.5f,0.5f,1.0f,0.0f,
        -0.5f,-0.5f,0.5f,0.0f,1.0f,
        0.5f,-0.5f,0.5f,1.0f,1.0f,
        -0.5f,-0.5f,0.5f,0.0f,1.0f,
        0.5f,0.5f,0.5f,1.0f,0.0f,
        -0.5f,0.5f,0.5f,1.0f,0.0f,
        -0.5f,0.5f,-0.5f,1.0f,1.0f,
        0.5f,0.5f,-0.5f,0.0f,1.0f,
        -0.5f,0.5f,0.5f,1.0f,0.0f,
        0.5f,0.5f,-0.5f,0.0f,1.0f,
        0.5f,0.5f,0.5f,0.0f,0.0f,
        -0.5f,0.5f,0.5f,1.0f,1.0f,
        -0.5f,-0.5f,0.5f,1.0f,0.0f,
        -0.5f,-0.5f,-0.5f,0.0f,0.0f,
        -0.5f,0.5f,0.5f,1.0f,1.0f,
        -0.5f,0.5f,-0.5f,0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,0.0f,0.0f,
        -0.5f,0.5f,-0.5f,0.0f,0.0f,
        0.5f,0.5f,-0.5f,1.0f,0.0f,
        -0.5f,-0.5f,-0.5f,0.0f,1.0f,
        0.5f,-0.5f,-0.5f,1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,0.0f,1.0f,
        0.5f,0.5f,-0.5f,1.0f,0.0f,
        -0.5f,-0.5f,0.5f,1.0f,0.0f,
        -0.5f,-0.5f,-0.5f,1.0f,1.0f,
        0.5f,-0.5f,-0.5f,0.0f,1.0f,
        -0.5f,-0.5f,0.5f,1.0f,0.0f,
        0.5f,-0.5f,-0.5f,0.0f,1.0f,
        0.5f,-0.5f,0.5f,0.0f,0.0f,
        0.5f,0.5f,0.5f,1.0f,1.0f,
        0.5f,-0.5f,0.5f,1.0f,0.0f,
        0.5f,-0.5f,-0.5f,0.0f,0.0f,
        0.5f,0.5f,0.5f,1.0f,1.0f,
        0.5f,0.5f,-0.5f,0.0f,1.0f,
        0.5f,-0.5f,-0.5f,0.0f,0.0f,

    };

    std::vector <std::string> textpath= {
        "C:/Users/Ramesh/Desktop/skybox/right.jpg",
        "C:/Users/Ramesh/Desktop/skybox/left.jpg",
        "C:/Users/Ramesh/Desktop/skybox/top.jpg",
        "C:/Users/Ramesh/Desktop/skybox/bottom.jpg",
        "C:/Users/Ramesh/Desktop/skybox/front.jpg",
        "C:/Users/Ramesh/Desktop/skybox/back.jpg"
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
    backTexture = loadCubemap(textpath);
    glEnable(GL_DEPTH_TEST);
    
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned char* data = stbi_load("C:/Users/Ramesh/Desktop/Cube3DTexture.jpg", &width, &height, &ColourChannels, 0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);


    glGenBuffers(1, &vboB);
    glGenVertexArrays(1, &vaoB);
    glBindVertexArray(vaoB);
    glBindBuffer(GL_ARRAY_BUFFER, vboB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(background), background, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    Shader cube("C:/Users/Ramesh/Desktop/Shaders/CubeVs.txt","C:/Users/Ramesh/Desktop/Shaders/CubeFs.txt");
    Shader skybox("C:/Users/Ramesh/Desktop/Shaders/skyboxVs.txt", "C:/Users/Ramesh/Desktop/Shaders/skyboxFs.txt");
    mat4 view,projection,model = mat4(1.0f);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthMask(GL_FALSE);
        skybox.use();
        view = mat4(mat3(lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
        skybox.setMat4("view", view);
        projection = perspective(radians(fov), (float)SCR_HEIGHT / (float)SCR_WIDTH, 0.1f, 100.0f);
        skybox.setMat4("projection", projection);
        glBindVertexArray(vaoB);
        glBindTexture(GL_TEXTURE_CUBE_MAP, backTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        cube.use();
        //model = translate(model, vec3(0.0f, 0.0f, 0.7f));
        cube.setMat4("model", model);
        view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        cube.setMat4("view", view);
        cube.setMat4("projection", projection);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
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
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
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

