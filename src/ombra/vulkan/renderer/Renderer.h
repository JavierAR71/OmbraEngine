//
// Created by Usuario on 31/07/2025.
//

#ifndef RENDERER_H
#define RENDERER_H



#pragma once

#include "core/OmbraCore.h"

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace ombra::vulkan {
    class Device;
    class Swapchain;
}

namespace ombra::renderer {
    class Renderer {
    public:
        Renderer(
            const vulkan::Device& device,
            VkPhysicalDevice physicalDevice,
            VkSurfaceKHR surface,
            uint32_t width,
            uint32_t height
        );
        ~Renderer();

        OMBRA_DISALLOW_COPY(Renderer);

        void beginFrame();
        void draw();
        void endFrame();

        bool isFrameInProgress() const { return m_frameStarted; }

    private:
        void createRenderPass();
        void createFramebuffers();
        void createCommandPool();
        void createCommandBuffers();
        void createSyncObjects();

    private:
        const vulkan::Device& m_device;

        std::unique_ptr<vulkan::Swapchain> m_swapchain;

        VkRenderPass m_renderPass = VK_NULL_HANDLE;
        std::vector<VkFramebuffer> m_framebuffers;

        VkCommandPool m_commandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_commandBuffers;

        size_t m_currentFrame = 0;
        bool m_frameStarted = false;

        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
    };
}

#endif //RENDERER_H
