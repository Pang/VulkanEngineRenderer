#include "app.hpp"
#include <stdexcept>
#include <array>

namespace graphicsEngine {
    App::App() {
        loadModels();
        createPipelineLayout();
        recreateSwapchain();
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

    void App::loadModels() {
        std::vector<Model::Vertex> vertices {
            { { 0.0, -0.5 }, { 1.0f, 0.0f, 0.0f } },
            { { 0.5, 0.5}, { 0.0f, 1.0f, 0.0f } },
            { {-0.5, 0.5}, { 0.0f, 0.0f, 1.0f } }
        };

        model = std::make_unique<Model>(device, vertices);
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
        auto pipelineConfig = PipelineSetup::defaultPipelineConfigInfo(swapChainSetup->width(), swapChainSetup->height());

        pipelineConfig.renderPass = swapChainSetup->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        pipelineSetup = std::make_unique<PipelineSetup>(
            device,
            "shaders/vert.spv",
            "shaders/frag.spv",
            pipelineConfig
        );
    }

    void App::recreateSwapchain() {
        auto extent = window.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = window.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device.device());
        swapChainSetup = std::make_unique<SwapChainSetup>(device, extent);
        createPipeline();
    }

    void App::createCommandBuffers() {
        commandBuffers.resize(swapChainSetup->imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers");
        }
    }

    void App::recordCommandBuffer(int imageIndex){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin command buffers");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChainSetup->getRenderPass();
        renderPassInfo.framebuffer = swapChainSetup->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChainSetup->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        pipelineSetup->bind(commandBuffers[imageIndex]);
        model->bind(commandBuffers[imageIndex]);
        model->draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer");
        }
    }

    void App::drawFrame() {
        uint32_t imageIndex;
        auto result = swapChainSetup->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapchain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image");
        }

        recordCommandBuffer(imageIndex);
        result = swapChainSetup->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()) {
            window.resetWindowResizeFlag();
            recreateSwapchain();
            return;
        }
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image");
        }
    }

}
