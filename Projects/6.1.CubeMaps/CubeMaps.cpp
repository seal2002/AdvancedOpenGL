// This is CubeMaps Demo from https://learnopengl.com/#!Advanced-OpenGL/Cubemaps
#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <GLFW\include\GLFW\glfw3.h>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\LoadTexture.h"
#include "common\CheckError.h"

#define PATH "..\\Projects\\6.1.CubeMaps"

using namespace OpenGLWindow;

static float SCR_W = 800.0f;
static float SCR_H = 600.0f;

void do_movement();
void changeEffect();
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
bool* keys;

// setup vertex data (and buffer(s)) and configure vertex attributes
// -----------------------------------------------------------------

float cubeVertices[] = {
        // positions          // texture Coords
        // Back face
    -0.5f, -0.5f, -0.5f,  // Bottom-left
     0.5f,  0.5f, -0.5f,  // top-right
     0.5f, -0.5f, -0.5f,  // bottom-right
     0.5f,  0.5f, -0.5f,  // top-right
    -0.5f, -0.5f, -0.5f,  // bottom-left
    -0.5f,  0.5f, -0.5f,  // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  // bottom-left
     0.5f, -0.5f,  0.5f,  // bottom-right
     0.5f,  0.5f,  0.5f,  // top-right
     0.5f,  0.5f,  0.5f,  // top-right
    -0.5f,  0.5f,  0.5f,  // top-left
    -0.5f, -0.5f,  0.5f,  // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  // top-right
    -0.5f,  0.5f, -0.5f,  // top-left
    -0.5f, -0.5f, -0.5f,  // bottom-left
    -0.5f, -0.5f, -0.5f,  // bottom-left
    -0.5f, -0.5f,  0.5f,  // bottom-right
    -0.5f,  0.5f,  0.5f,  // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  // top-left
     0.5f, -0.5f, -0.5f,  // bottom-right
     0.5f,  0.5f, -0.5f,  // top-right
     0.5f, -0.5f, -0.5f,  // bottom-right
     0.5f,  0.5f,  0.5f,  // top-left
     0.5f, -0.5f,  0.5f,  // bottom-left
    // Bottom face
    -0.5f, -0.5f, -0.5f,  // top-right
     0.5f, -0.5f, -0.5f,  // top-left
     0.5f, -0.5f,  0.5f,  // bottom-left
     0.5f, -0.5f,  0.5f,  // bottom-left
    -0.5f, -0.5f,  0.5f,  // bottom-right
    -0.5f, -0.5f, -0.5f,  // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  // top-left
     0.5f,  0.5f,  0.5f,  // bottom-right
     0.5f,  0.5f, -0.5f,  // top-right
     0.5f,  0.5f,  0.5f,  // bottom-right
    -0.5f,  0.5f, -0.5f,  // top-left
    -0.5f,  0.5f,  0.5f,  // bottom-left
};

int main()
{
    Window window(SCR_W, SCR_H, "CubeMaps Demo");
    char* path = new char[50];
    sprintf(path, "%s\\%s", PATH, "CubeMap");
    Shader ourShader(path);
    glEnable(GL_DEPTH_TEST);
    // Init VAO, VBO for Cube
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glCheckError();

    // load textures
    // -------------
    vector<string> faces
    {
        "..\\Resources\\container.jpg",
        "..\\Resources\\container.jpg",
        "..\\Resources\\container.jpg",
        "..\\Resources\\container.jpg",
        "..\\Resources\\container.jpg",
        "..\\Resources\\container.jpg"
    };

    unsigned int cubemapTexture = loadTextureCubeMap(faces);
    glCheckError();

    ourShader.Use();
    ourShader.setInt("cube", 0);

    while (!window.shouldClose())
    {
        keys = window.getKeyPress();
        do_movement();

        // render
        // -----

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw Scence
        // -----------------------------------------------------------

        ourShader.Use();
        glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(45.0f, (float)(SCR_W / SCR_H), 0.1f, 100.0f);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // -----------------------------------------------------------

        window.pollEvents();
        window.swapBuffers();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
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