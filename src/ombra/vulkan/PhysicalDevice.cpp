#include "vulkan/PhysicalDevice.h"
#include "Config.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>

namespace ombra::vulkan {

PhysicalDevice::PhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
    : m_instance(instance), m_surface(surface) {
    pickPhysicalDevice();
}

void PhysicalDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("[PhysicalDevice] No GPUs with Vulkan support found.");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            m_device = device;
            findQueueFamilies(device);
            break;
        }
    }

    if (m_device == VK_NULL_HANDLE) {
        throw std::runtime_error("[PhysicalDevice] No suitable GPU found.");
    }

    if (ombra::ENABLE_LOGGING) {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(m_device, &props);

        std::cout << "[PhysicalDevice] Selected GPU: " << props.deviceName << std::endl;
        std::cout << "  - API Version: " << VK_VERSION_MAJOR(props.apiVersion) << "."
                                         << VK_VERSION_MINOR(props.apiVersion) << "."
                                         << VK_VERSION_PATCH(props.apiVersion) << std::endl;
    }
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
    std::optional<uint32_t> graphicsIndex;
    std::optional<uint32_t> presentIndex;

    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueCount; ++i) {
        const auto& props = queueFamilies[i];

        if (props.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsIndex = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

        if (presentSupport) {
            presentIndex = i;
        }

        if (graphicsIndex.has_value() && presentIndex.has_value()) {
            break;
        }
    }

    return graphicsIndex.has_value() && presentIndex.has_value();
}

void PhysicalDevice::findQueueFamilies(VkPhysicalDevice device) {
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueCount; ++i) {
        if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !m_graphicsQueueFamily.has_value()) {
            m_graphicsQueueFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
        if (presentSupport && !m_presentQueueFamily.has_value()) {
            m_presentQueueFamily = i;
        }

        if (m_graphicsQueueFamily && m_presentQueueFamily) {
            return;
        }
    }

    throw std::runtime_error("[PhysicalDevice] Failed to find required queue families.");
}

}
