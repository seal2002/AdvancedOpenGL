// Instancing Demo from https://learnopengl.com/#!Advanced-OpenGL/Instancing
#include <queue>
#include <stack>
#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <GLFW\include\GLFW\glfw3.h>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\LoadTexture.h"
#include "common\CheckError.h"
#include "common\TextRendering.h"

#include "Model.h"

#define PATH "..\\Projects\\10.3.AsteroidField"

#define MAX_INSTANCES 1000

using namespace OpenGLWindow;

const static int SCR_W = 1280;
const static int SCR_H = 720;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void do_movement();
Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));
bool* keys;

void main()
{
    Window window(SCR_W, SCR_H, "Asteroid Field", false);

    glEnable(GL_DEPTH_TEST);
    // Config for shader
    string path(PATH);
    Shader shader(string(path + "\\AsteroidField").c_str());
    Shader text(string(path + "\\text").c_str());

    // VBO for the instance Arrays
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[MAX_INSTANCES];
    srand(glfwGetTime()); // initial ramdom seed
    float radius = 50.0f;
    float offset = 2.5f;
    for (int i = 0; i < MAX_INSTANCES; i++)
    {
        glm::mat4 model;
        // 1. translation: displace a long circle with 'radius' in the rand [-offset, offset]
        float angle = (float)i / (float)MAX_INSTANCES * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotate axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    Model planet("..\\Resources\\planet\\planet.obj");
    Model rock("..\\Resources\\rock\\rock.obj");

    // Init Text
    TextRendering textRendering(SCR_W, SCR_H);
	float _time[5];
	std::stack<float> time;

    while(!window.shouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float FPS = 1.0f / deltaTime;
        keys = window.getKeyPress();
        do_movement();
        // Render// draw Cube
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_W) / float(SCR_H), 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.Use();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        shader.setMat4("model", model);
        planet.Draw(shader);
		_time[0] = glfwGetTime();
        for(int i = 0; i < MAX_INSTANCES; i++)
        {
            model = modelMatrices[i];
            shader.setMat4("model", model);
            rock.Draw(shader);
        }
		_time[1] = glfwGetTime();
		time.push(float(_time[1] - _time[0]));

        // Render FPS 
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        string textFPS("FPS = ");
        textFPS += to_string(FPS);
        textRendering.RenderText(text, textFPS, 25.0f, SCR_H - 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
		textFPS.clear();
		textFPS = string("Render MAX_INSTANCES = ");
		textFPS += to_string(time.top());
		time.pop();
		textRendering.RenderText(text, textFPS, 25.0f, SCR_H - 50.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
		textFPS.clear();
		textFPS = string("Render TEXT = ");
		textFPS += to_string(time.empty() ? 0 : time.top());
		textRendering.RenderText(text, textFPS, 25.0f, SCR_H - 75.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
		textFPS.clear();
		textFPS = string("Swap Buffer = ");
		textFPS += to_string(time.empty() ? 0 : time.top());
		textRendering.RenderText(text, textFPS, 25.0f, SCR_H - 100.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        textFPS.clear();
        window.pollEvents();
		_time[2] = glfwGetTime();
		time.push(float(_time[2] - _time[1]));
        window.swapBuffers();
		_time[3] = glfwGetTime();
		time.push(float(_time[3] - _time[2]));
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