//
// Created by Usuario on 30/07/2025.
//

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "vulkan/Instance.h"
#include "vulkan/DebugMessenger.h"
#include "vulkan/Surface.h"
#include "vulkan/PhysicalDevice.h"
#include "renderer/Renderer.h"

struct GLFWwindow;

namespace ombra {

    class Application {
    public:
        Application();
        ~Application();

        void run();

    private:
        void initWindow();

        const uint32_t WIDTH = 1280;
        const uint32_t HEIGHT = 720;

        GLFWwindow* window = nullptr;

        std::unique_ptr<vulkan::Instance> m_instance;
        std::unique_ptr<vulkan::DebugMessenger> m_debugMessenger;
        std::unique_ptr<vulkan::Surface> m_surface;
        std::unique_ptr<vulkan::PhysicalDevice> m_physicalDevice;
        std::unique_ptr<vulkan::Device> m_device;
        std::unique_ptr<renderer::Renderer> m_renderer;
    };

    };


