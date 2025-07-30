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
#include "Config.h"
#include "Device.h"
#include "PhysicalDevice.h"

namespace ombra {

    Application::Application() {
        initWindow();

        m_instance = std::make_unique<vulkan::Instance>(
            "Ombra Engine", OMBRA_ENGINE_VERSION
        );

        m_debugMessenger = std::make_unique<vulkan::DebugMessenger>(
            m_instance->get()
        );

        m_surface = std::make_unique<vulkan::Surface>(
            m_instance->get(), window
        );

        m_physicalDevice = std::make_unique<vulkan::PhysicalDevice>(
            m_instance->get(), m_surface->get()
        );

        m_device = std::make_unique<vulkan::Device>(
        m_physicalDevice->get(),
         m_surface->get()
        );

        m_renderer = std::make_unique<renderer::Renderer>(
        *m_device,
        m_physicalDevice->get(),
        m_surface->get(),
        WIDTH,
        HEIGHT
);
    }

    Application::~Application() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Application::run() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            m_renderer->beginFrame();
            m_renderer->draw();
            m_renderer->endFrame();
        }
    }

    void Application::initWindow() {
        if (!glfwInit()) {
            throw std::runtime_error("[Application] Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Ombra Engine", nullptr, nullptr);
        if (!window) {
            std::cerr << "[Application] Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
    }

}



