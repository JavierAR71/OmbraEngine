//
// Created by Usuario on 31/07/2025.
//

#ifndef DEVICE_H
#define DEVICE_H

#pragma once

#include "core/OmbraCore.h"

#include <vulkan/vulkan.h>
#include <optional>

namespace ombra::vulkan {

    class Device {
    public:
        Device(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
        ~Device();

        OMBRA_DISALLOW_COPY(Device);

        VkDevice get() const { return m_device; }
        VkQueue getGraphicsQueue() const { return m_graphicsQueue; }
        VkQueue getPresentQueue() const { return m_presentQueue; }
        uint32_t getGraphicsQueueFamily() const {
            return m_graphicsQueueFamily.value();
        }

    private:
        void createLogicalDevice();
        void pickQueueFamilies();

        VkDevice m_device = VK_NULL_HANDLE;

        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue = VK_NULL_HANDLE;

        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;

        std::optional<uint32_t> m_graphicsQueueFamily;
        std::optional<uint32_t> m_presentQueueFamily;

        const std::vector<const char*> m_requiredExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    };

}


#endif //DEVICE_H
