//
// Created by Usuario on 31/07/2025.
//

#include "vulkan/Device.h"
#include "core/OmbraCore.h"

#include <stdexcept>
#include <iostream>
#include <set>

namespace ombra::vulkan {

Device::Device(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
    : m_physicalDevice(physicalDevice), m_surface(surface) {

    pickQueueFamilies();
    createLogicalDevice();
}

Device::~Device() {
    if (m_device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_device, nullptr);
        if (ombra::ENABLE_LOGGING) {
            std::cout << "[Device] Vulkan logical device destroyed" << std::endl;
        }
    }
}

void Device::pickQueueFamilies() {
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, nullptr);

    std::vector<VkQueueFamilyProperties> families(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, families.data());

    for (uint32_t i = 0; i < queueCount; ++i) {
        if ((families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !m_graphicsQueueFamily.has_value()) {
            m_graphicsQueueFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);
        if (presentSupport && !m_presentQueueFamily.has_value()) {
            m_presentQueueFamily = i;
        }

        if (m_graphicsQueueFamily && m_presentQueueFamily) break;
    }

    if (!m_graphicsQueueFamily || !m_presentQueueFamily) {
        throw std::runtime_error("[Device] Failed to find required queue families.");
    }
}

void Device::createLogicalDevice() {
    std::set<uint32_t> uniqueQueues = {
        m_graphicsQueueFamily.value(),
        m_presentQueueFamily.value()
    };

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float priority = 1.0f;

    for (uint32_t queueFamily : uniqueQueues) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &priority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = m_requiredExtensions.data();

    if (ombra::ENABLE_VALIDATION_LAYERS) {
        static const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
    }

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("[Device] Failed to create logical device.");
    }

    vkGetDeviceQueue(m_device, m_graphicsQueueFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, m_presentQueueFamily.value(), 0, &m_presentQueue);

    if (ombra::ENABLE_LOGGING) {
        std::cout << "[Device] Vulkan logical device created" << std::endl;
    }
}

}

