// This is Face Culling Demo from https://learnopengl.com/#!Advanced-OpenGL/Face-culling
#include <MainWindow.hpp>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <GLFW\include\GLFW\glfw3.h>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\LoadTexture.h"

#define PATH "..\\Projects\\4.FaceCulling"

using namespace OpenGLWindow;

static float SCR_W = 800.0f;
static float SCR_H = 600.0f;

void do_movement();
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
bool* keys;

// setup vertex data (and buffer(s)) and configure vertex attributes
// -----------------------------------------------------------------

float grassVertices[] = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

 float marbleVertices[] = {
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

float groundVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    -5.0f, 0.0f, 5.0f, 0.0f, 0.0f,
     5.0f, 0.0f, 5.0f, 2.0f, 0.0f,
     5.0f, 0.0f, -5.0f, 2.0f, 2.0f,

     5.0f, 0.0f, -5.0f, 2.0f, 2.0f,
    -5.0f, 0.0f, -5.0f, 0.0f, 2.0f,
    -5.0f, 0.0f, 5.0f, 0.0f, 0.0f,

};

int main()
{
    Window window(SCR_W, SCR_H, "Face Culling Demo");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    char* path = new char[50];
    sprintf(path, "%s\\%s", PATH, "FaceCulling");
    Shader ourShader(path);

    // Init VAO, VBO for Grass
    unsigned int grassVAO, grassVBO;
    glGenBuffers(1, &grassVBO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), &grassVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &grassVAO);
    glBindVertexArray(grassVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glBindVertexArray(0);

    // Init VAO, VBO for Marble
    unsigned int marbleVAO, marbleVBO;
    glGenBuffers(1, &marbleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, marbleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(marbleVertices), &marbleVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &marbleVAO);
    glBindVertexArray(marbleVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glBindVertexArray(0);

    // Init VAO, VBO for Ground
    unsigned int groundVAO, groundVBO;
    glGenBuffers(1, &groundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), &groundVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &groundVAO);
    glBindVertexArray(groundVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int grassTexture = loadTexture("..\\Resources\\grass.png");
    unsigned int marbleTexture = loadTexture("..\\Resources\\marble.jpg");
    unsigned int groundTexture = loadTexture("..\\Resources\\metal.png");

    // Config for grass possition
    vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    // shader configuration
    ourShader.Use();
    ourShader.setInt("ourTexture", 0);

    glCullFace(GL_BACK);
    while (!window.shouldClose())
    {
        keys = window.getKeyPress();
        do_movement();

        // render
        // -----
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.Use();

        glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(45.0f, (float)(SCR_W / SCR_H), 0.1f, 100.0f);;

        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // ground
        glBindVertexArray(groundVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        ourShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // marble cubes
        glBindVertexArray(marbleVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, marbleTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.5f, -1.0f));
        ourShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(2.0f, 0.5f, 0.0f));
        ourShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // grass
        glBindVertexArray(grassVAO);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        for (unsigned int i = 0; i < vegetation.size(); i++)
        {
            model = glm::mat4();
            model = glm::translate(model, vegetation[i]);
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

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