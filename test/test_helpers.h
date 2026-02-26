#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdexcept>

class GlfwTestContext {
public:
    GlfwTestContext() : window_(nullptr) {
        if (ref_count_ == 0) {
            if (!glfwInit()) {
                throw std::runtime_error("glfwInit failed");
            }
        }
        ref_count_++;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        window_ = glfwCreateWindow(64, 64, "test", nullptr, nullptr);
        if (!window_) {
            ref_count_--;
            if (ref_count_ == 0) {
                glfwTerminate();
            }
            throw std::runtime_error("glfwCreateWindow failed");
        }

        glfwMakeContextCurrent(window_);
        glfwSwapInterval(0);

        if (!glad_loaded_) {
            if (!gladLoadGL()) {
                glfwDestroyWindow(window_);
                window_ = nullptr;
                ref_count_--;
                if (ref_count_ == 0) {
                    glfwTerminate();
                }
                throw std::runtime_error("gladLoadGL failed");
            }
            glad_loaded_ = true;
        }
    }

    ~GlfwTestContext() {
        if (window_) {
            glfwDestroyWindow(window_);
            window_ = nullptr;
        }

        ref_count_--;
        if (ref_count_ == 0) {
            glfwTerminate();
            glad_loaded_ = false;
        }
    }

    GLFWwindow* window() const { return window_; }

private:
    GLFWwindow* window_;
    inline static int ref_count_ = 0;
    inline static bool glad_loaded_ = false;
};
