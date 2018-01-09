// This is CubeMaps & SkyBox Demo from https://learnopengl.com/#!Advanced-OpenGL/Cubemaps
#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <GLFW\include\GLFW\glfw3.h>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\LoadTexture.h"
#include "common\CheckError.h"
#include "common\TextRendering.h"

#define PATH "..\\Projects\\6.2.Skybox"

using namespace OpenGLWindow;

const static int SCR_W = 800;
const static int SCR_H = 600;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void do_movement();
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
bool* keys;

// setup vertex data (and buffer(s)) and configure vertex attributes

float cubeVertices[] = {
        // positions          // texture Coords
        // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
};

float skyboxVertices[] = {
    // positions
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

int main()
{
    Window window(SCR_W, SCR_H, "SkyBox Demo");

    glEnable(GL_DEPTH_TEST);
    // Load Shader
    char *path = new char[50];
    sprintf(path, "%s\\%s", PATH, "cube");
    Shader cube(path);
    sprintf(path, "%s\\%s", PATH, "skybox");
    Shader skybox(path);

    Shader textShader("..\\Projects\\6.2.Skybox\\text.vs", "..\\Projects\\6.2.Skybox\\text.fs");
    // Init VAO, VBO for Cube
    unsigned int cubeVAO, cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glBindVertexArray(0);
    glCheckError();
    // Init VAO, VBO for SkyBox
    unsigned int skyboxVAO, skyboxVBO;
    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glBindVertexArray(0);
    glCheckError();

    // Init Text
    TextRendering textRendering(SCR_W, SCR_H);
    // Load Texture
    unsigned int textureMarble = loadTexture("..\\Resources\\marble.jpg");

    vector<string> faces = {
        "..\\Resources\\skybox\\right.jpg",
        "..\\Resources\\skybox\\left.jpg",
        "..\\Resources\\skybox\\top.jpg",
        "..\\Resources\\skybox\\bottom.jpg",
        "..\\Resources\\skybox\\back.jpg",
        "..\\Resources\\skybox\\front.jpg"
    };

    unsigned int skyboxTexture = loadTextureCubeMap(faces);

    // Configuration Shader

    cube.Use();
    cube.setInt("texture1", 0);

    skybox.Use();
    skybox.setInt("skybox", 0);
    // Init textFPS
    char textFPS[50];
    while(!window.shouldClose())
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float FPS = 1.0f / deltaTime;

        keys = window.getKeyPress();
        do_movement();
        // Render

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw Cube
        glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(45.0f, (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        cube.Use();
        cube.setMat4("model", model);
        cube.setMat4("view", view);
        cube.setMat4("projection", projection);

        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureMarble);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // draw SkyBox

        glDepthFunc(GL_LEQUAL);
        skybox.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skybox.setMat4("view",view);
        skybox.setMat4("projection", projection);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        sprintf(textFPS, "FPS = %f", FPS);
        textRendering.RenderText(textShader, textFPS, 25.0f, SCR_H - 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        window.pollEvents();
        window.swapBuffers();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);
}

void do_movement()
{
    if (keys[GLFW_KEY_W])
    {
        camera.ProcessKeyboard(Camera_Movement::FORWARD);
    }
    if (keys[GLFW_KEY_S])
    {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD);
    }
    if (keys[GLFW_KEY_A])
    {
        camera.ProcessKeyboard(Camera_Movement::LEFT);
    }
    if (keys[GLFW_KEY_D])
    {
        camera.ProcessKeyboard(Camera_Movement::RIGHT);
    }
    if (keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(Camera_Movement::ROTATE_LEFT);
    }
    if (keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(Camera_Movement::ROTATE_RIGHT);
    }
    if (keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(Camera_Movement::ROTATE_UP);
    }
    if (keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(Camera_Movement::ROTATE_DOWN);
    }
}