// Instancing Demo from https://learnopengl.com/#!Advanced-OpenGL/Instancing
#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <GLFW\include\GLFW\glfw3.h>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\LoadTexture.h"
#include "common\CheckError.h"

#include "Model.h"

#define PATH "..\\Projects\\10.4.AsteroidInstanced"

#define MAX_INSTANCES 1000

using namespace OpenGLWindow;

const static int SCR_W = 1280;
const static int SCR_H = 720;

void do_movement();
Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));
bool* keys;

void main()
{
    Window window(SCR_W, SCR_H, "Asteroid Field");

    glEnable(GL_DEPTH_TEST);
    // Config for shader
    string path(PATH);
    path += "\\asteroidinstanced";
    Shader shaderInstanced(path.c_str());

    string pathPlanet(PATH);
    pathPlanet += "\\planet";
    Shader shader(pathPlanet.c_str());

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

    // vertex Buffer Object
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(glm::mat4), & modelMatrices[0], GL_STATIC_DRAW);

    for(int i = 0; i < rock.meshes.size(); i++)
    {
        unsigned int VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        // vertex Attibutes
        GLsizei vec4size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)(vec4size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)(2 * vec4size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)(3 * vec4size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glBindVertexArray(0);
    }
    glCheckError();

    while(!window.shouldClose())
    {
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

        shaderInstanced.Use();
        shaderInstanced.setMat4("projection", projection);
        shaderInstanced.setMat4("view", view);
        for(int i = 0; i < rock.meshes.size(); i++)
        {
            glBindVertexArray(rock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, MAX_INSTANCES);
        }
        glCheckError();
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