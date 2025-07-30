//
// Created by Usuario on 30/07/2025.
//

#include <memory>
#include <vulkan/vulkan.h>
#include <vulkan/instance.h>
#include <string>
#include <vector>

#include "vulkan/DebugMessenger.h"
#include <memory>

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
        };

    };


