//
// Created by Usuario on 30/07/2025.
//

#include "vulkan/DebugMessenger.h"
#include "Config.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Instance.h"

namespace ombra::vulkan {
    DebugMessenger::DebugMessenger(VkInstance instance)
    : m_instance(instance) {
        if (!ombra::ENABLE_VALIDATION_LAYERS) {
            return;
        }

        setupMessenger();
    }

    DebugMessenger::~DebugMessenger() {
        if (m_debugMessenger != VK_NULL_HANDLE) {
            auto destroyFunc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));

            if (destroyFunc) {
                destroyFunc(m_instance, m_debugMessenger, nullptr);
            }
        }
    }

    void DebugMessenger::setupMessenger() {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;

        auto createFunc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));

        if (createFunc == nullptr) {
            throw std::runtime_error("[DebugMessenger] vkCreateDebugUtilsMessengerEXT is nullptr — extension might be missing.");
        }

        VkResult result = createFunc(m_instance, &createInfo, nullptr, &m_debugMessenger);
        if (result != VK_SUCCESS) {
            std::cerr << "[DebugMessenger] Failed to create debug messenger. VkResult = " << result << std::endl;
            throw std::runtime_error("Failed to create debug messenger.");
        }
    }

    VkBool32 VKAPI_CALL DebugMessenger::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT        messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* /*pUserData*/) {

        std::cerr << "[Vulkan] validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
} // ombra