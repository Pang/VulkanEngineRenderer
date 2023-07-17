#pragma once
#include "window.hpp"
#include "deviceSetup.hpp"
#include "pipelineSetup.hpp"
#include "swapChainSetup.hpp"
#include "model.hpp"

#include <memory>
#include <vector>

namespace graphicsEngine {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App();
		~App();

		App(const App&) = delete;
		void operator=(const App&) = delete;

		void run();
		void recreateSwapchain();
		void recordCommandBuffer(int imageIndex);

	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		Window window{ WIDTH, HEIGHT, "Vulkan" };
		DeviceSetup device{ window };
		std::unique_ptr<SwapChainSetup> swapChainSetup;
		std::unique_ptr<PipelineSetup> pipelineSetup;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<Model> model;
	};
}