// window_manager.cpp
// Implementation of window management functionality

#include "window/window_manager.h"
#include <GLFW/glfw3.h>
#include <cstdio>

namespace blec {
namespace window {

WindowManager::WindowManager() : window_(nullptr), glfw_initialized_(false) {}

WindowManager::~WindowManager() {
    Shutdown();
}

bool WindowManager::InitializeGLFW() {
    if (glfw_initialized_) {
        return true;
    }

    if (!glfwInit()) {
        std::fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfw_initialized_ = true;
    return true;
}

bool WindowManager::CreateWindow(int width, int height, const char* title) {
    if (!glfw_initialized_) {
        std::fprintf(stderr, "GLFW not initialized\n");
        return false;
    }

    // Request OpenGL 2.1 for maximum compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Request a depth buffer for 3D rendering
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        std::fprintf(stderr, "Failed to create GLFW window\n");
        return false;
    }

    return true;
}

void WindowManager::MakeContextCurrent() {
    if (window_) {
        glfwMakeContextCurrent(window_);
    }
}

void WindowManager::SetVSync(int interval) {
    glfwSwapInterval(interval);
}

bool WindowManager::ShouldClose() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void WindowManager::SetShouldClose(bool shouldClose) {
    if (window_) {
        glfwSetWindowShouldClose(window_, shouldClose ? GLFW_TRUE : GLFW_FALSE);
    }
}

void WindowManager::PollEvents() {
    glfwPollEvents();
}

void WindowManager::SwapBuffers() {
    if (window_) {
        glfwSwapBuffers(window_);
    }
}

void WindowManager::GetFramebufferSize(int* width, int* height) const {
    if (window_) {
        glfwGetFramebufferSize(window_, width, height);
    } else {
        if (width) *width = 0;
        if (height) *height = 0;
    }
}

void WindowManager::Shutdown() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }

    if (glfw_initialized_) {
        glfwTerminate();
        glfw_initialized_ = false;
    }
}

} // namespace window
} // namespace blec
