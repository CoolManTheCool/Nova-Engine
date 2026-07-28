#include "compute_pipeline.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

#include <iostream>
#include <iterator>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "utility/config.hpp"

namespace Nova {
ComputePipeline::ComputePipeline(Device& device, const std::string& shader, DescriptorSetLayout& layout, VkPushConstantRange pushConstantRange)
    : device{device}, descriptorSetLayout{layout}, pushConstantRange{pushConstantRange} {

    createPipelineLayout();
    createComputePipeline(shader);
}

const std::vector<char>& getShader(const std::string& name);

VkShaderModule ComputePipeline::createShaderModule(const std::string& shader) {
    const std::vector<char>& code = getShader(shader);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(device.device(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }

    return shaderModule;
}

void Nova::ComputePipeline::createPipelineLayout() {
    VkDescriptorSetLayout layout = descriptorSetLayout.getDescriptorSetLayout();

    VkPipelineLayoutCreateInfo info{};

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    info.setLayoutCount = 1;
    info.pSetLayouts    = &layout;

    if (pushConstantRange.size > 0) {
        info.pushConstantRangeCount = 1;
        info.pPushConstantRanges    = &pushConstantRange;
    }

    if (vkCreatePipelineLayout(
            device.device(),
            &info,
            nullptr,
            &pipelineLayout
        ) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create compute pipeline layout"
        );
    }
}

void Nova::ComputePipeline::createComputePipeline(
    const std::string& shader
) {

    VkShaderModule shaderModule = createShaderModule(shader);

    VkPipelineShaderStageCreateInfo stageInfo{};
    stageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.stage  = VK_SHADER_STAGE_COMPUTE_BIT;
    stageInfo.module = shaderModule;
    stageInfo.pName  = "main";

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage  = stageInfo;
    pipelineInfo.layout = pipelineLayout;

    if (vkCreateComputePipelines(
            device.device(),
            VK_NULL_HANDLE,
            1,
            &pipelineInfo,
            nullptr,
            &pipeline
        ) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute pipeline");
    }

    vkDestroyShaderModule(
        device.device(),
        shaderModule,
        nullptr
    );
}

void Nova::ComputePipeline::bind(VkCommandBuffer commandBuffer) const {
    vkCmdBindPipeline(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        pipeline
    );
}

void ComputePipeline::dispatch(
    VkCommandBuffer      cmd,
    const DescriptorSet& set,
    uint32_t             x,
    const void*          pushData
) {

    bind(cmd);

    VkDescriptorSet descriptorSet = set.get();

    vkCmdBindDescriptorSets(
        cmd,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        pipelineLayout,
        0,
        1,
        &descriptorSet,
        0,
        nullptr
    );

    if (pushData != nullptr && pushConstantRange.size > 0) {
        vkCmdPushConstants(
            cmd,
            pipelineLayout,
            pushConstantRange.stageFlags,
            pushConstantRange.offset,
            pushConstantRange.size,
            pushData
        );
    }

    vkCmdDispatch(cmd, x, 1, 1);
}

const std::vector<char>& getShader(const std::string& name) {
    static std::unordered_map<std::string, std::vector<char>> cache;

    auto it = cache.find(name);
    if (it != cache.end())
        return it->second;

    std::string path =
        getExecutableDirectory() +
        "/resources/shaders/" +
        name +
        ".spv";

    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader: " + path);
    }

    size_t size = static_cast<size_t>(file.tellg());

    std::vector<char> buffer(size);

    file.seekg(0);

    file.read(buffer.data(), size);

    auto [inserted, success] = cache.emplace(name, std::move(buffer));

    return inserted->second;
}

} // namespace Nova