#include "pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include "settings.hpp"

namespace nova {
    std::vector<char> nova_PipeLine::readFile(const std::string& filepath) {

        if (filepath.empty()) {
            throw std::runtime_error("File path is empty");
        }

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

    nova_PipeLine::nova_PipeLine(nova_Device &device) : device{device} {
        createGraphicsPipeLine(Settings.vertFilepath, Settings.fragFilepath);
    }

    void nova_PipeLine::createGraphicsPipeLine(const std::string& vertFilepath,
    const std::string& fragFilepath) {
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
}