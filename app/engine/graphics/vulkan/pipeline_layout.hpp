#pragma once

#include "device.hpp"
#include "descriptors.hpp"

#include <vector>

namespace Nova {

class PipelineLayout {
public:
    class Builder {
    public:
        Builder(Device& device);

        Builder& addDescriptorSetLayout(DescriptorSetLayout& layout);

        Builder& addPushConstantRange(
            VkShaderStageFlags stageFlags,
            uint32_t           size,
            uint32_t           offset = 0);

        std::unique_ptr<PipelineLayout> build() const;

    private:
        Device& device;

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{};
        std::vector<VkPushConstantRange>   pushConstantRanges{};
    };

    PipelineLayout(
        Device&                                   device,
        const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
        const std::vector<VkPushConstantRange>&   pushConstantRanges);

    ~PipelineLayout();

    PipelineLayout(const PipelineLayout&)            = delete;
    PipelineLayout& operator=(const PipelineLayout&) = delete;

    VkPipelineLayout get() const {
        return pipelineLayout;
    }

private:
    Device& device;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};

} // namespace Nova