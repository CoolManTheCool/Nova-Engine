#include "descriptors.hpp"

#include <cassert>
#include <stdexcept>

namespace Nova {

DescriptorSetLayout::Builder& DescriptorSetLayout::Builder::addBinding(
    uint32_t           binding,
    VkDescriptorType   descriptorType,
    VkShaderStageFlags stageFlags,
    uint32_t           count
) {

    assert(bindings.count(binding) == 0 && "Binding already in use");

    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding         = binding;
    layoutBinding.descriptorType  = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags      = stageFlags;

    bindings[binding] = layoutBinding;

    return *this;
}

std::unique_ptr<DescriptorSetLayout> DescriptorSetLayout::Builder::build() const {
    return std::make_unique<DescriptorSetLayout>(device, bindings);
}

DescriptorSetLayout::DescriptorSetLayout(
    Device&                                                           device,
    const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings
)
    // This is tragically long and complicated and idk what to do about it
    : device{device}, bindings{bindings} {

    for (const auto& kv : bindings) {
        layoutBindings.push_back(kv.second);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
    layoutInfo.pBindings    = layoutBindings.data();

    if (vkCreateDescriptorSetLayout(
            device.device(),
            &layoutInfo,
            nullptr,
            &descriptorSetLayout
        ) != VK_SUCCESS) {

        throw std::runtime_error("Failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::~DescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr);
}

DescriptorPool::Builder& DescriptorPool::Builder::addPoolSize(
    VkDescriptorType descriptorType,
    uint32_t         count
) {

    poolSizes.push_back({descriptorType, count});

    return *this;
}

DescriptorPool::Builder& DescriptorPool::Builder::setPoolFlags(
    VkDescriptorPoolCreateFlags flags
) {

    poolFlags = flags;

    return *this;
}

DescriptorPool::Builder& DescriptorPool::Builder::setMaxSets(uint32_t count) {
    maxSets = count;

    return *this;
}

std::unique_ptr<DescriptorPool> DescriptorPool::Builder::build() const {
    return std::make_unique<DescriptorPool>(
        device,
        maxSets,
        poolFlags,
        poolSizes
    );
}

DescriptorPool::DescriptorPool(
    Device&                                  device,
    uint32_t                                 maxSets,
    VkDescriptorPoolCreateFlags              poolFlags,
    const std::vector<VkDescriptorPoolSize>& poolSizes
)
    : device{device} {

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes    = poolSizes.data();
    poolInfo.maxSets       = maxSets;
    poolInfo.flags         = poolFlags;

    if (vkCreateDescriptorPool(
            device.device(),
            &poolInfo,
            nullptr,
            &descriptorPool
        ) != VK_SUCCESS) {

        throw std::runtime_error("Failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool() {
    vkDestroyDescriptorPool(
        device.device(),
        descriptorPool,
        nullptr
    );
}

bool DescriptorPool::allocateDescriptor(
    VkDescriptorSetLayout descriptorSetLayout,
    VkDescriptorSet&      descriptor
) const {

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool     = descriptorPool;
    allocInfo.pSetLayouts        = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    return vkAllocateDescriptorSets(
               device.device(),
               &allocInfo,
               &descriptor
           ) == VK_SUCCESS;
}

void DescriptorPool::freeDescriptors(
    std::vector<VkDescriptorSet>& descriptors
) const {

    vkFreeDescriptorSets(
        device.device(),
        descriptorPool,
        static_cast<uint32_t>(descriptors.size()),
        descriptors.data()
    );
}

void DescriptorPool::resetPool() {
    vkResetDescriptorPool(
        device.device(),
        descriptorPool,
        0
    );
}

DescriptorWriter::DescriptorWriter(
    DescriptorSetLayout& setLayout,
    DescriptorPool&      pool
)
    : setLayout(setLayout), pool(pool) {

    bufferInfos.reserve(10);
    imageInfos.reserve(10);
    writes.reserve(10);
}

DescriptorWriter& DescriptorWriter::writeBuffer(
    uint32_t                      binding,
    const VkDescriptorBufferInfo& bufferInfo
) {
    bufferInfos.push_back(bufferInfo);

    VkWriteDescriptorSet write{};
    write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding      = binding;
    write.descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    write.descriptorCount = 1;

    writes.push_back(write);

    return *this;
}

DescriptorWriter& DescriptorWriter::writeImage(
    uint32_t                     binding,
    const VkDescriptorImageInfo& imageInfo
) {

    assert(
        setLayout.bindings.count(binding) == 1 &&
        "Layout does not contain specified binding"
    );

    auto& bindingDescription = setLayout.bindings[binding];

    assert(
        bindingDescription.descriptorCount == 1 &&
        "Binding expects multiple descriptors"
    );

    imageInfos.push_back(imageInfo);

    VkWriteDescriptorSet write{};
    write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding      = binding;
    write.descriptorType  = bindingDescription.descriptorType;
    write.descriptorCount = 1;
    write.pImageInfo      = &imageInfos.back();
    writes.push_back(write);

    return *this;
}

bool DescriptorWriter::build(
    VkDescriptorSet& set
) {

    if (!pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set)) {
        return false;
    }

    overwrite(set);

    return true;
}

void DescriptorWriter::overwrite(VkDescriptorSet set) {
    for (size_t i = 0; i < writes.size(); i++) {
        writes[i].pBufferInfo = &bufferInfos[i];
        writes[i].dstSet = set;
    }

    vkUpdateDescriptorSets(
        pool.device.device(),
        static_cast<uint32_t>(writes.size()),
        writes.data(),
        0,
        nullptr
    );
}

DescriptorSet::DescriptorSet(DescriptorPool& pool, DescriptorSetLayout& layout)
    : pool{pool}, layout{layout} {

    if (!pool.allocateDescriptor(layout.getDescriptorSetLayout(), descriptorSet)) {
        throw std::runtime_error("Failed to allocate descriptor set");
    }
}

void DescriptorSet::bind(VkCommandBuffer cmd, VkPipelineLayout layout, uint32_t set) const {
    vkCmdBindDescriptorSets(
        cmd,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        layout,
        set,
        1,
        &descriptorSet,
        0,
        nullptr
    );
}

} // namespace Nova