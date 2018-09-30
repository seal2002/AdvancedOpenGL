// This is demo from https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing

#include <MainWindow.hpp>

#include "common\Camera.h"
#include "common\Shader.h"
#include "common\Object.h"
#include "common\CheckError.h"

#define PATH "..\\Projects\\11.2.OffScreenMSAA"
#define NUMBER_OF_SAMPLE 4

using namespace OpenGLWindow;

static float SCR_W = 800.0f;
static float SCR_H = 600.0f;

Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));

Object *Cube;
Object *Screen;

void main()
{
    Window window(SCR_W, SCR_H, "Off Screen MSAA");

    glEnable(GL_DEPTH_TEST);

    Cube = new Object();
    Cube->Load("..\\Resources\\Cube.txt");
    Screen = new Object();
    Screen->Load("..\\Resources\\Screen.txt", true);

    string path(PATH);
    Shader shader(string(path + "\\shader").c_str());
    Shader screenShader(string(path + "\\screen").c_str());

    // configure MSAA framebuffer
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // create a multisampled color attachment texture
    unsigned int textureColorBufferMultiSampled;
    glGenTextures(1, &textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, NUMBER_OF_SAMPLE, GL_RGB, SCR_W, SCR_H, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
    
    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    unsigned int renderBufferObj;
    glGenRenderbuffers(1, &renderBufferObj);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObj);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, NUMBER_OF_SAMPLE, GL_DEPTH24_STENCIL8, SCR_W, SCR_H);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObj);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure second post-processing framebuffer
    unsigned int intermediateFBO;
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    // create a color attachment texture
    unsigned int screenTexture;
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_W, SCR_H, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Intermediate Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();
    // shader configuration

    shader.Use();
	screenShader.Use();
    screenShader.setInt("screenTexture", 0);
	glCheckError();

    while(!window.shouldClose())
    {
        window.doMovement(&camera);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // 1. draw scene as normal in multisampled buffers
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 projection = glm::perspective(camera.zoom, (float) SCR_W / (float) SCR_H, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.Use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // Draw Cube
        Cube->Render(shader, glm::mat4());
		glCheckError();
        // 2. now blit multisampled buffers to normal colorbuffer of intermediate FBO. Imange is stored in screenTexture
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glCheckError();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glCheckError();
        glBlitFramebuffer(0, 0, SCR_W, SCR_H, 0, 0, SCR_W, SCR_H, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glCheckError();
        // 3. new render quad with sence's visuals as its texture image
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
		glCheckError();
        // draw screen quad
        screenShader.Use();
		

        // bind screen texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTexture); // use the now resolved color attachment as the quad's texture
		glCheckError();
        // Draw texture screen to Screen
        Screen->Render(screenShader, glm::mat4());
		glCheckError();

        window.swapBuffers();
        window.pollEvents();
    }
}

