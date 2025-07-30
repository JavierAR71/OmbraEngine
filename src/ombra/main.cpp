//
// Created by Usuario on 30/07/2025.
//

#include "vulkan/vulkan.h"
#include <iostream>
#include <stdexcept>
#include "Application.h"

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
    try {
        ombra::Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "[Ombra] Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
//
