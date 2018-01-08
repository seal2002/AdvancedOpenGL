#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <GLFW\include\GLFW\glfw3.h>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\LoadTexture.h"
#include "common\CheckError.h"
#include "common\TextRendering.h"

using namespace OpenGLWindow;

const static int SCR_W = 800;
const static int SCR_H = 600;

//void do_movement();
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
bool* keys;

int main()
{
    Window window(SCR_W, SCR_H, "Advance GLSL - Uniform Buffer Demo");

    // Init Shader for text
    Shader textShader("..\\Projects\\TextRendering\\text.vs", "..\\Projects\\TextRendering\\text.fs");

    // Init Text Rendering class
    TextRendering textRendering(SCR_W, SCR_H);

    while (!window.shouldClose())
    {
        keys = window.getKeyPress();
        // Set OpenGL options
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        textRendering.RenderText(textShader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        window.pollEvents();
        window.swapBuffers();
    }
}