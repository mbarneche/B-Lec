/**
 * @file application.cpp
 * @brief Main application implementation
 */

#include "core/application.h"
#include "rendering/renderer.h"
#include "world/world.h"
#include "world/block_interaction.h"
#include "world/block_selector.h"
#include "camera/camera.h"
#include "input/input_manager.h"
#include "ui/ui_system.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

// Forward declarations for GLFW callbacks
static Application* g_app_instance = nullptr;

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    if (g_app_instance && width > 0 && height > 0) {
        g_app_instance->OnWindowResize(static_cast<uint32_t>(width), 
                                        static_cast<uint32_t>(height));
    }
}

static void ErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

Application::Application()
    : window_(nullptr),
      is_running_(false),
      window_width_(1280),
      window_height_(720),
      window_title_("B-Lec - Electricity Simulator"),
      last_frame_time_(0.0) {
    g_app_instance = this;
}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    // Initialize GLFW
    glfwSetErrorCallback(ErrorCallback);
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Set OpenGL context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window_ = glfwCreateWindow(window_width_, window_height_, window_title_, nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable vsync
    glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

    // Initialize systems
    renderer_ = std::make_unique<Renderer>();
    if (!renderer_->Initialize(window_width_, window_height_)) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }

    world_ = std::make_unique<World>();
    world_->Initialize();

    camera_ = std::make_unique<Camera>(
        glm::vec3(0.0f, 32.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    camera_->SetAspectRatio(static_cast<float>(window_width_) / window_height_);

    input_manager_ = std::make_unique<InputManager>(window_);
    
    ui_system_ = std::make_unique<UISystem>();
    ui_system_->Initialize(window_width_, window_height_);

    block_selector_ = std::make_unique<BlockSelector>();
    block_selector_->Initialize();

    block_interaction_ = std::make_unique<BlockInteraction>();
    block_interaction_->Initialize(6.0f);
    block_interaction_->SetSelectedBlock(block_selector_->GetSelectedBlock());

    is_running_ = true;
    last_frame_time_ = glfwGetTime();

    std::cout << "Application initialized successfully" << std::endl;
    return true;
}

void Application::Run() {
    while (is_running_) {
        double current_time = glfwGetTime();
        float delta_time = static_cast<float>(current_time - last_frame_time_);
        last_frame_time_ = current_time;

        ProcessInput();
        Update(delta_time);
        Render();

        glfwSwapBuffers(window_);
        glfwPollEvents();

        if (glfwWindowShouldClose(window_)) {
            is_running_ = false;
        }
    }
}

void Application::Shutdown() {
    block_interaction_.reset();
    block_selector_.reset();
    ui_system_.reset();
    input_manager_.reset();
    camera_.reset();
    world_.reset();
    renderer_.reset();

    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();

    std::cout << "Application shut down" << std::endl;
}

bool Application::IsRunning() const {
    return is_running_;
}

void Application::RequestClose() {
    is_running_ = false;
}

void Application::OnWindowResize(uint32_t width, uint32_t height) {
    window_width_ = width;
    window_height_ = height;
    
    // Update OpenGL viewport
    glViewport(0, 0, width, height);
    
    // Update camera aspect ratio
    if (camera_) {
        camera_->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
    
    // TODO: Update UI system if it needs resize notification
    // TODO: Update renderer if needed
}

void Application::ProcessInput() {
    input_manager_->Update();

    if (input_manager_->IsKeyJustPressed(Key::ESC)) {
        RequestClose();
    }

    // Block selection - number keys (1-0 for slots 0-9)
    for (int i = 0; i < 10; ++i) {
        // Key codes: GLFW_KEY_1 through GLFW_KEY_0
        if (glfwGetKey(window_, GLFW_KEY_1 + i) == GLFW_PRESS) {
            block_selector_->SelectByHotbar(i);
            block_interaction_->SetSelectedBlock(block_selector_->GetSelectedBlock());
        }
    }

    // Block selection - mouse wheel scroll
    double mouse_x, mouse_y;
    glfwGetCursorPos(window_, &mouse_x, &mouse_y);
    static double last_scroll_y = 0.0;
    
    // TODO: Implement proper scroll callback for block selection
    if (input_manager_->IsKeyJustPressed(Key::E)) {
        block_selector_->SetMenuVisible(!block_selector_->IsMenuVisible());
    }

    // Camera movement - use actual delta time from Run() loop
    // Note: Create a member variable for this or pass it as parameter
    float move_speed = 32.0f;
    double current_time = glfwGetTime();
    float delta_time = static_cast<float>(current_time - last_frame_time_);
    
    if (input_manager_->IsKeyPressed(Key::W)) {
        camera_->MoveForward(move_speed * delta_time);
    }
    if (input_manager_->IsKeyPressed(Key::S)) {
        camera_->MoveForward(-move_speed * delta_time);
    }
    if (input_manager_->IsKeyPressed(Key::D)) {
        camera_->MoveRight(move_speed * delta_time);
    }
    if (input_manager_->IsKeyPressed(Key::A)) {
        camera_->MoveRight(-move_speed * delta_time);
    }
    if (input_manager_->IsKeyPressed(Key::SPACE)) {
        camera_->MoveUp(move_speed * delta_time);
    }
    if (input_manager_->IsKeyPressed(Key::SHIFT)) {
        camera_->MoveUp(-move_speed * delta_time);
    }

    // Block interaction
    bool left_click = input_manager_->IsMouseButtonPressed(0);
    bool right_click = input_manager_->IsMouseButtonPressed(1);
    block_interaction_->Update(world_.get(), camera_.get(), left_click, right_click);
}

void Application::Update(float delta_time) {
    camera_->Update(delta_time);
    world_->Update(delta_time);
    ui_system_->Update(delta_time);
    ui_system_->SetCameraPosition(camera_->GetPosition());
    
    // Safely get block info with null check
    const BlockInfo* block_info = BlockRegistry::GetBlockInfo(block_selector_->GetSelectedBlock());
    if (block_info) {
        ui_system_->SetSelectedBlock(block_info->name);
    } else {
        ui_system_->SetSelectedBlock("Unknown");
    }
}

void Application::Render() {
    renderer_->Clear();
    renderer_->Render(world_.get(), camera_.get());
    ui_system_->Render();
}
