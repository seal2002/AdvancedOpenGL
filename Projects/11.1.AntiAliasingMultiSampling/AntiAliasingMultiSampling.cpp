// This is demo from https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing

#include <MainWindow.hpp>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\Object.h"

#define PATH "..\\Projects\\11.1.AntiAliasingMultiSampling"

using namespace OpenGLWindow;

static float SCR_W = 800.0f;
static float SCR_H = 600.0f;

Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));

Object *Cube;

int main()
{
    Window window(SCR_W, SCR_H, "Anti Aliasing Multi Sampling");

    glEnable(GL_DEPTH_TEST);

    Cube = new Object();
    Cube->Load("..\\Resources\\Cube.txt");
    
    string path(PATH);
    Shader shader(string(path + "\\shader").c_str());

    while(!window.shouldClose())
    {
        window.doMovement(&camera);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(camera.zoom, (float) SCR_W / (float) SCR_H, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.Use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 model;
        // Draw Cube
        Cube->Render(shader, model);

        window.swapBuffers();
        window.pollEvents();
        //window.processInput();
    }
}