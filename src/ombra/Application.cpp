//
// Created by Usuario on 30/07/2025.
//

#include "Application.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string.h>
#include <vulkan/vulkan.h>
#include <vulkan/instance.h>
#include "Config.h""

namespace ombra {

    Application::Application() {
        initWindow(); // glfwInit() debe ir primero

        m_instance = std::make_unique<vulkan::Instance>("Ombra Engine", VK_MAKE_VERSION(0, 1, 0));

        m_debugMessenger = std::make_unique<vulkan::DebugMessenger>(
            m_instance->get()
        );
    }

    Application::~Application() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Application::run() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void Application::initWindow() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Ombra Engine", nullptr, nullptr);

        if (window == nullptr) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
    }
}



