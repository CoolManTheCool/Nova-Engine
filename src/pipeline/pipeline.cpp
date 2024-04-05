#include <pipeline.hpp>

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace nova {
    std::vector<char> nova_PipeLine::readFile(const std::string& filepath) {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};
        if(!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        return buffer;
    }

    nova_PipeLine::nova_PipeLine(
    nova_Device &device,
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipeLineConfigInfo configInfo) : device{device} {
        createGraphicsPipeLine(vertFilepath, fragFilepath, configInfo);
    }

    void nova_PipeLine::createGraphicsPipeLine(const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipeLineConfigInfo configInfo) {
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << "\n";
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << "\n";
    }

    void nova_PipeLine::createShaderModule(const std::vector<char>& code, VkShaderModule * shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }
    }

    PipeLineConfigInfo nova_PipeLine::defaultPipeLineConfigInfo(uint32_t width, uint32_t height) {
        PipeLineConfigInfo configInfo{};

        return configInfo;
    }
}