#include "pipeline_layout.hpp"

#include <stdexcept>

namespace Nova {

PipelineLayout::Builder::Builder(Device& device) : device(device) {
}

PipelineLayout::Builder& PipelineLayout::Builder::addDescriptorSetLayout(DescriptorSetLayout& layout) {
    descriptorSetLayouts.push_back(layout.getDescriptorSetLayout());

    return *this;
}

PipelineLayout::Builder& PipelineLayout::Builder::addPushConstantRange(
    VkShaderStageFlags stageFlags,
    uint32_t           size,
    uint32_t           offset) {

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = stageFlags;
    pushConstantRange.offset     = offset;
    pushConstantRange.size       = size;

    pushConstantRanges.push_back(pushConstantRange);

    return *this;
}

std::unique_ptr<PipelineLayout> PipelineLayout::Builder::build() const {
    return std::make_unique<PipelineLayout>(
        device,
        descriptorSetLayouts,
        pushConstantRanges);
}

PipelineLayout::PipelineLayout(
    Device&                                   device,
    const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
    const std::vector<VkPushConstantRange>&   pushConstantRanges)
    : device{device} {

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount         = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts            = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
    pipelineLayoutInfo.pPushConstantRanges    = pushConstantRanges.data();

    if (vkCreatePipelineLayout(
            device.device(),
            &pipelineLayoutInfo,
            nullptr,
            &pipelineLayout) != VK_SUCCESS) {

        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

PipelineLayout::~PipelineLayout() {
    vkDestroyPipelineLayout(
        device.device(),
        pipelineLayout,
        nullptr);
}

} // namespace Nova