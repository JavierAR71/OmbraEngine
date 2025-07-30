//
// Created by Usuario on 31/07/2025.
//

#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#pragma once

#include "core/OmbraCore.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace ombra::vulkan {

    class Swapchain {
    public:
        Swapchain(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            VkSurfaceKHR surface,
            uint32_t windowWidth,
            uint32_t windowHeight
        );
        ~Swapchain();

        OMBRA_DISALLOW_COPY(Swapchain);

        VkSwapchainKHR get() const { return m_swapchain; }
        VkFormat getImageFormat() const { return m_imageFormat; }
        VkExtent2D getExtent() const { return m_extent; }
        const std::vector<VkImageView>& getImageViews() const { return m_imageViews; }

    private:
        void createSwapchain();
        void createImageViews();

        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;

        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

        std::vector<VkImage> m_images;
        std::vector<VkImageView> m_imageViews;

        VkFormat m_imageFormat = VK_FORMAT_UNDEFINED;
        VkExtent2D m_extent = {};

        uint32_t m_windowWidth = 0;
        uint32_t m_windowHeight = 0;
    };

}

#endif //SWAPCHAIN_H
