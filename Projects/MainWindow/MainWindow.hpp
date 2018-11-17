#ifndef MAINWINDOW_LIB_HPP
#define MAINWINDOW_LIB_HPP 1

#include <string>
#include <memory>
#include <iostream>

#include <glad/glad.h>
#include "common/Camera.h"

// Forward declare GLFWwindow to avoid including glfw3.h
struct GLFWwindow;

namespace OpenGLWindow
{
    class Window
    {
    public:
        Window(int width, int  height, const std::string& title, bool antiAlias = false);
        ~Window();

        bool shouldClose() const noexcept;

        void pollEvents() const noexcept;

        void swapBuffers() const noexcept;

        void processInput() const noexcept;

        std::pair<int, int> getWindowSize() const noexcept;

        bool * getKeyPress() const noexcept;

        bool * getKeyRelease() const noexcept;

        void doMovement(Camera *camera);

        GLFWwindow* wnd;
    };
}

#endif