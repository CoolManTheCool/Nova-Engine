#pragma once

#include "graphics/vulkan/device.hpp"
#include "graphics/vulkan/descriptors.hpp"

namespace Nova {

class ComputePipeline {
public:
    ComputePipeline(Device& device, const std::string& shader, DescriptorSetLayout& layout, VkPushConstantRange pushConstantRange);

    void bind(VkCommandBuffer commandBuffer) const;

    void dispatch(
        VkCommandBuffer      cmd,
        const DescriptorSet& set,
        uint32_t             x,
        const void*          pushData
    );

    void dispatch(
        VkCommandBuffer      cmd,
        const DescriptorSet& set,
        uint32_t             x,
        uint32_t             y,
        uint32_t             z,
        const void*          pushData
    );

private:
    VkShaderModule createShaderModule(const std::string& shader);

    void createPipelineLayout();
    void createComputePipeline(const std::string& shader);

    Device& device;

    DescriptorSetLayout& descriptorSetLayout;

    VkPushConstantRange pushConstantRange;

    VkPipeline       pipeline       = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};

} // namespace Nova