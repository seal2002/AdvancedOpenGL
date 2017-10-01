// Advance GLSL Demo from https://learnopengl.com/#!Advanced-OpenGL/Advanced-GLSL
#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <GLFW\include\GLFW\glfw3.h>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\LoadTexture.h"
#include "common\CheckError.h"
#define PATH "..\\Projects\\10.2.InstancesArrays"
#define MAX_INSTANCES 100

using namespace OpenGLWindow;

const static int SCR_W = 800;
const static int SCR_H = 600;

void do_movement();
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
bool* keys;

float quadVertices[] = {
    // positions // colors
    -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
    0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

    -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
    0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
    0.05f, 0.05f, 0.0f, 1.0f, 1.0f
};

void main()
{
    Window window(SCR_W, SCR_H, "Intances Arrays Quads");

    // Config for shader

    string path(PATH);
    path += "\\instancesArrays";
    Shader shader(path.c_str());

    // VBO for the instance Arrays
    glm::vec2 translations[MAX_INSTANCES];
    int index = 0;
    float offset = 0.1f;

    for(int y = -10; y < 10; y += 2)
    {
        for(int x = -10; x < 10;  x+= 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MAX_INSTANCES, &translations[0], GL_STATIC_DRAW);

    // VAO, VBO for Quad
    unsigned int quadVAO, quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
    // Set vertex attribute pointer and enable the vertex attribute
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);

    while(!window.shouldClose())
    {
        keys = window.getKeyPress();
        do_movement();
        // Render// draw Cube
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();

        glBindVertexArray(quadVAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MAX_INSTANCES);
        glBindVertexArray(0);

        window.pollEvents();
        window.swapBuffers();
    }

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