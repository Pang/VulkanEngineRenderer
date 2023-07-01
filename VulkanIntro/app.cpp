#include "app.hpp"
#include <stdexcept>
#include <array>

namespace graphicsEngine {
    App::App() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }
    App::~App() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void App::run() {
        while (!window.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(device.device());
	}

    void App::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void App::createPipeline() {
        auto pipelineConfig = PipelineSetup::defaultPipelineConfigInfo(swapChainSetup.width(), swapChainSetup.height());

        pipelineConfig.renderPass = swapChainSetup.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        pipelineSetup = std::make_unique<PipelineSetup>(
            device,
            "shaders/vert.spv",
            "shaders/frag.spv",
            pipelineConfig
        );
    }

    void App::createCommandBuffers() {
        commandBuffers.resize(swapChainSetup.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers");
        }

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Failed to begin command buffers");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChainSetup.getRenderPass();
            renderPassInfo.framebuffer = swapChainSetup.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChainSetup.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            pipelineSetup->bind(commandBuffers[i]);
            vkCmdDraw(commandBuffers[i], 3, 1, 1, 1);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to record command buffer");
            }
        }
    }

    void App::drawFrame() {
        uint32_t imageIndex;
        auto result = swapChainSetup.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image");
        }

        result = swapChainSetup.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image");
        }
    }

}
