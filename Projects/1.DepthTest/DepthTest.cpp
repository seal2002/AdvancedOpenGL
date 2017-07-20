#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\include\GLFW\glfw3.h>
#include "common\Shader.h"
#include "common\Camera.h"

using namespace OpenGLWindow;

void do_movement();
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool* keys;

static float SCR_W = 800.0f;
static float SCR_H = 600.0f;

int main()
{
    Window window(SCR_W, SCR_H, "Depth Test !!!");

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("..\\Resource\\1.Depth_testing.vs","..\\Resource\\1.Depth_testing.fs");

    while(!window.shouldClose())
    {
        keys = window.getKeyPress();
        do_movement();
        // render
        // -----
        glClearColor(0.2f, 0.3f, 0.4f, 0.0f);
    }
}