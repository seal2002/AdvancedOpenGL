// Advance GLSL Demo from https://learnopengl.com/#!Advanced-OpenGL/Advanced-GLSL
#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <GLFW\include\GLFW\glfw3.h>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\LoadTexture.h"
#include "common\CheckError.h"

#include "Model.h"

#define PATH "..\\Projects\\9.3.ExplodingObjects"

using namespace OpenGLWindow;

const static int SCR_W = 1280;
const static int SCR_H = 720;

void do_movement();
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
bool* keys;

void main()
{
    Window window(SCR_W, SCR_H, "Exploding Objects");

    glEnable(GL_DEPTH_TEST);
    // Config for Shader

    string pathVS(PATH),pathFS(PATH),pathGS(PATH);
    pathVS += "\\loadingModelShader.vs";
    pathFS += "\\loadingModelShader.fs";
    pathGS += "\\loadingModelShader.gs";
    Shader shader(pathVS.c_str(), pathFS.c_str(), pathGS.c_str());

    Model ourModel("..\\Resources\\nanosuit\\nanosuit.obj");

    while(!window.shouldClose())
    {
        keys = window.getKeyPress();
        do_movement();
        // Render// draw Cube
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_W) / float(SCR_H), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        shader.setFloat("time", glfwGetTime());

        ourModel.Draw(shader);
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