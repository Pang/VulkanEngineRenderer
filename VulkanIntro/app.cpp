#include "app.hpp"
#include <stdexcept>

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
        }
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

    }

    void App::drawFrame() {

    }

}
