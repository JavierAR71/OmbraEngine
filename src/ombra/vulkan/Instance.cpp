
#include "Instance.h"
#include "Config.h"
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <GLFW/glfw3.h>

namespace ombra::vulkan {

    Instance::Instance(const std::string &applicationName, uint32_t applicationVersion) {
        if (ombra::ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName.c_str();
        appInfo.applicationVersion = applicationVersion;
        appInfo.pEngineName = "Ombra";
        appInfo.engineVersion = VK_MAKE_VERSION(ombra::ENGINE_VERSION_MAJOR
            , ombra::ENGINE_VERSION_MINOR
            , ombra::ENGINE_VERSION_PATCH);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();

        if (ombra::ENABLE_VALIDATION_LAYERS) {
            std::cout << "Validation layers enabled\n";
        } else {
            std::cout << "Validation layers DISABLED\n";
        }

        std::cout << "Extensions used:\n";
        for (const auto& ext : extensions) {
            std::cout << "  - " << ext << std::endl;
        }

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data(); // ← ¡esto debe ir DESPUÉS del push_back!

        if (ombra::ENABLE_VALIDATION_LAYERS) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
            createInfo.ppEnabledLayerNames = m_validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }

        std::cout << "Vulkan instance created" << std::endl;
    }

    Instance::~Instance() {
        if (m_instance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_instance, nullptr);
            std::cout << "Vulkan instance destroyed" << std::endl;
        }
    }

    bool Instance::checkValidationLayerSupport() const {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> Instance::getRequiredExtensions() const {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (ombra::ENABLE_VALIDATION_LAYERS) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }


}