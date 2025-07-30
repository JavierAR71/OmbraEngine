//
// Created by Usuario on 31/07/2025.
//

#pragma once

#include "renderer/Renderer.h"
#include "core/OmbraCore.h"

#include "vulkan/Device.h"
#include "vulkan/Swapchain.h"

#include <stdexcept>
#include <array>
#include <iostream>

namespace ombra::renderer {
    Renderer::Renderer(
        const vulkan::Device& device,
        VkPhysicalDevice physicalDevice,
        VkSurfaceKHR surface,
        uint32_t width,
        uint32_t height)
        : m_device(device)
    {
        m_swapchain = std::make_unique<vulkan::Swapchain>(
            physicalDevice, m_device.get(), surface, width, height);

        createRenderPass();
        createFramebuffers();
        createCommandPool();
        createCommandBuffers();
        createSyncObjects();

        if (ombra::ENABLE_LOGGING) {
            std::cout << "[Renderer] Initialized" << std::endl;
        }
    }

    Renderer::~Renderer() {
        vkDestroyCommandPool(m_device.get(), m_commandPool, nullptr);

        for (auto framebuffer : m_framebuffers) {
            vkDestroyFramebuffer(m_device.get(), framebuffer, nullptr);
        }

        vkDestroyRenderPass(m_device.get(), m_renderPass, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            vkDestroySemaphore(m_device.get(), m_imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(m_device.get(), m_renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(m_device.get(), m_inFlightFences[i], nullptr);
        }

        if (ombra::ENABLE_LOGGING) {
            std::cout << "[Renderer] Destroyed" << std::endl;
        }
    }

    void Renderer::createRenderPass() {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = m_swapchain->getImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorRef = {};
        colorRef.attachment = 0;
        colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorRef;

        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &colorAttachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;

        if (vkCreateRenderPass(m_device.get(), &info, nullptr, &m_renderPass) != VK_SUCCESS) {
            throw std::runtime_error("[Renderer] Failed to create render pass.");
        }
    }

    void Renderer::createFramebuffers() {
        auto views = m_swapchain->getImageViews();
        m_framebuffers.resize(views.size());

        for (size_t i = 0; i < views.size(); ++i) {
            VkImageView attachments[] = { views[i] };

            VkFramebufferCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            info.renderPass = m_renderPass;
            info.attachmentCount = 1;
            info.pAttachments = attachments;
            info.width = m_swapchain->getExtent().width;
            info.height = m_swapchain->getExtent().height;
            info.layers = 1;

            if (vkCreateFramebuffer(m_device.get(), &info, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("[Renderer] Failed to create framebuffer.");
            }
        }
    }

    void Renderer::createCommandPool() {
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = m_device.getGraphicsQueueFamily();

        if (vkCreateCommandPool(m_device.get(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
            throw std::runtime_error("[Renderer] Failed to create command pool.");
        }
    }

    void Renderer::createCommandBuffers() {
        m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

        if (vkAllocateCommandBuffers(m_device.get(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("[Renderer] Failed to allocate command buffers.");
        }
    }

    void Renderer::createSyncObjects() {
        m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            if (vkCreateSemaphore(m_device.get(), &semInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_device.get(), &semInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_device.get(), &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("[Renderer] Failed to create sync objects.");
                }
        }
    }

    void Renderer::beginFrame() {
        vkWaitForFences(m_device.get(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(m_device.get(), 1, &m_inFlightFences[m_currentFrame]);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(
            m_device.get(),
            m_swapchain->get(),
            UINT64_MAX,
            m_imageAvailableSemaphores[m_currentFrame],
            VK_NULL_HANDLE,
            &imageIndex
        );

        // Reset command buffer
        vkResetCommandBuffer(m_commandBuffers[m_currentFrame], 0);

        // Begin recording
        VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        vkBeginCommandBuffer(m_commandBuffers[m_currentFrame], &beginInfo);

        // Begin render pass
        VkClearValue clearColor = { {{0.1f, 0.1f, 0.1f, 1.0f}} }; // gris oscuro

        VkRenderPassBeginInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        renderPassInfo.renderPass = m_renderPass;
        renderPassInfo.framebuffer = m_framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_swapchain->getExtent();
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_commandBuffers[m_currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        m_frameStarted = true;
    }

    void Renderer::draw() {
        if (!m_frameStarted) return;

        // (No hay contenido aún)

        vkCmdEndRenderPass(m_commandBuffers[m_currentFrame]);
    }

    void Renderer::endFrame() {
        if (!m_frameStarted) return;

        vkEndCommandBuffer(m_commandBuffers[m_currentFrame]);

        VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

        VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffers[m_currentFrame];

        VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_device.getGraphicsQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("[Renderer] Failed to submit draw command buffer.");
        }

        // Present
        VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapchains[] = { m_swapchain->get() };
        uint32_t imageIndex = 0;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(m_device.getPresentQueue(), &presentInfo);

        m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        m_frameStarted = false;
    }







}

