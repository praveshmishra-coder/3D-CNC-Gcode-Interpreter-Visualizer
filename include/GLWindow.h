#pragma once
#include <GLFW/glfw3.h>
#include <stdexcept>

class GLWindow {
    GLFWwindow* window;

public:
    GLWindow(int w, int h, const char* title) {
        if (!glfwInit())
            throw std::runtime_error("GLFW init failed");

        window = glfwCreateWindow(w, h, title, nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Window creation failed");
        }

        glfwMakeContextCurrent(window);
    }

    ~GLWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow* get() const { return window; }
};
