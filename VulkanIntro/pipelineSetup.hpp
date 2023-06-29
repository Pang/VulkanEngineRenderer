#pragma once

#include "deviceSetup.hpp"

#include <string>
#include <vector>

namespace graphicsEngine {

	struct PipelineConfigInfo {};

	class PipelineSetup {
	public:
		PipelineSetup(DeviceSetup &device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& info);
		~PipelineSetup() {}

		PipelineSetup(const PipelineSetup&) = delete;
		PipelineSetup& operator=(const PipelineSetup&) = delete;

		static std::vector<char> readFile(const std::string& filePath);
		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& info);
		
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		DeviceSetup& device;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}