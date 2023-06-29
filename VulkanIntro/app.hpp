#pragma once
#include "window.hpp"
#include "deviceSetup.hpp"
#include "pipelineSetup.hpp"

namespace graphicsEngine {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		Window window{ WIDTH, HEIGHT, "Vulkan" };
		DeviceSetup device{ window };
		PipelineSetup pipelineSetup{ device, "shaders/vert.spv", "shaders/frag.spv", PipelineSetup::defaultPipelineConfigInfo(WIDTH, HEIGHT) };
	};
}