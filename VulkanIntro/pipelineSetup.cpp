#pragma once

#define GLFW_INCLUDE_VULKAN

#include "pipelineSetup.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <GLFW/glfw3.h>

namespace graphicsEngine {

    PipelineSetup::PipelineSetup(DeviceSetup& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& info)
        : device { device } {
        createGraphicsPipeline(vertFilePath, fragFilePath, info);
    }

	std::vector<char> PipelineSetup::readFile(const std::string& filePath) {
        std::ifstream file {filePath, std::ios::ate | std::ios::binary };

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
	}

    PipelineConfigInfo PipelineSetup::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo info{};
        return info;
    }

    void PipelineSetup::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& info) {
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        std::cout << "Vertex Shader Code size: " << vertCode.size() << '\n';
        std::cout << "Frag Shader Code size: " << vertCode.size() << '\n';
    }

    void PipelineSetup::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }

        std::cout << "Shader Module created\n";
    }
}