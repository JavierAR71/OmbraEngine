//
// Created by Usuario on 30/07/2025.
//

#include "vulkan/vulkan.h"
#include <iostream>

namespace ombra {
    void printVulkanVersion() {
        uint32_t version = VK_API_VERSION_1_0;
        VkResult result = vkEnumerateInstanceVersion(&version);

        if (result == VK_SUCCESS) {
            std::cout << "Vulkan version: " << version << std::endl;
        } else {
            std::cout << "Failed to retrieve Vulkan version" << std::endl;
        }
    }
}

int main() {
    ombra::printVulkanVersion();
    return 0;
}
//
