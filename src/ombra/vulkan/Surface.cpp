#include "vulkan/Surface.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

namespace ombra::vulkan {

    Surface::Surface(VkInstance instance, GLFWwindow* window)
        : m_instance(instance) {

        if (glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }

        std::cout << "[Surface] Window surface created" << std::endl;
    }

    Surface::~Surface() {
        if (m_surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
            std::cout << "[Surface] Window surface destroyed" << std::endl;
        }
    }

}