//
// Created by Usuario on 30/07/2025.
//

#ifndef DEBUGMESSENGER_H
#define DEBUGMESSENGER_H
#include <vulkan/vulkan.h>

namespace ombra::vulkan {
    class DebugMessenger {
    public:
        DebugMessenger(VkInstance instance);
        ~DebugMessenger();

        DebugMessenger(const DebugMessenger&) = delete;
        DebugMessenger& operator=(const DebugMessenger&) = delete;

    private:
        VkInstance m_instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

        void setupMessenger();
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    };

} // ombra

#endif //DEBUGMESSENGER_H
