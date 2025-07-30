
#ifndef PHYSICALDEVICE_H
#define PHYSICALDEVICE_H

#pragma once

#include <vulkan/vulkan.h>
#include <optional>
#include "core/Macros.h"

namespace ombra::vulkan {

    class PhysicalDevice {
    public:
        PhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
        ~PhysicalDevice() = default;

        OMBRA_DISALLOW_COPY(PhysicalDevice);

        VkPhysicalDevice get() const { return m_device; }

        uint32_t getGraphicsQueueFamily() const { return m_graphicsQueueFamily.value(); }
        uint32_t getPresentQueueFamily() const { return m_presentQueueFamily.value(); }

    private:
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        void findQueueFamilies(VkPhysicalDevice device);

        VkInstance m_instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        VkPhysicalDevice m_device = VK_NULL_HANDLE;

        std::optional<uint32_t> m_graphicsQueueFamily;
        std::optional<uint32_t> m_presentQueueFamily;
    };

}


#endif //PHYSICALDEVICE_H
