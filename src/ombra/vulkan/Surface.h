//
// Created by Usuario on 30/07/2025.
//

#ifndef SURFACE_H
#define SURFACE_H

#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace ombra::vulkan {
    class Surface {
    public:
        Surface(VkInstance instance, GLFWwindow* window);
        ~Surface();

        Surface(const Surface&) = delete;
        Surface& operator=(const Surface&) = delete;

        VkSurfaceKHR get() const { return m_surface; }

    private:
        VkInstance m_instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    };
}

#endif //SURFACE_H
