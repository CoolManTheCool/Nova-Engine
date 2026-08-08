#include "compute.hpp"

#include "graphics/vulkan/device.hpp"

#include <stdexcept>

namespace Nova {

ComputeContext::ComputeContext(Device& device)
    : device(device) {
    queue = device.presentQueue();

    VkFenceCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    vkCreateFence(
        device.device(),
        &info,
        nullptr,
        &fence
    );

    createCommandPool();
}

ComputeContext::~ComputeContext() {
    vkDestroyCommandPool(
        device.device(),
        commandPool,
        nullptr
    );

    vkDestroyFence(device.device(), fence, nullptr);
}

void ComputeContext::createCommandPool() {
    VkCommandPoolCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    info.queueFamilyIndex = device.findPhysicalQueueFamilies().computeFamily;

    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(
            device.device(),
            &info,
            nullptr,
            &commandPool
        ) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create compute command pool"
        );
    }
}

VkCommandBuffer ComputeContext::begin() {
    VkCommandBufferAllocateInfo alloc{};
    alloc.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc.commandPool        = commandPool;
    alloc.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc.commandBufferCount = 1;

    VkCommandBuffer buffer;

    vkAllocateCommandBuffers(
        device.device(),
        &alloc,
        &buffer
    );

    VkCommandBufferBeginInfo begin{};
    begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(buffer, &begin);

    return buffer;
}

void ComputeContext::copyBuffer(
    VkCommandBuffer cmd,
    Buffer&         src,
    Buffer&         dst,
    VkDeviceSize    size
) {

    VkBufferMemoryBarrier barrier{};
    barrier.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    barrier.srcAccessMask       = VK_ACCESS_SHADER_WRITE_BIT;
    barrier.dstAccessMask       = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.buffer              = src.getBuffer();
    barrier.offset              = 0;
    barrier.size                = size;

    vkCmdPipelineBarrier(
        cmd,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr,
        1, &barrier,
        0, nullptr
    );

    VkBufferCopy region{};
    region.size = size;

    vkCmdCopyBuffer(
        cmd,
        src.getBuffer(),
        dst.getBuffer(),
        1,
        &region
    );
}

void ComputeContext::clearBuffer(
    VkCommandBuffer cmd,
    Buffer&         buffer,
    uint32_t        value
) {

    vkCmdFillBuffer(
        cmd,
        buffer.getBuffer(),
        0,
        buffer.getBufferSize(),
        value
    );
}

void ComputeContext::submit(VkCommandBuffer commandBuffer) {
    VkSubmitInfo submit{};
    submit.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers    = &commandBuffer;

    vkEndCommandBuffer(commandBuffer);

    vkQueueSubmit(
        queue,
        1,
        &submit,
        fence
    );

    vkWaitForFences(
        device.device(),
        1,
        &fence,
        VK_TRUE,
        UINT64_MAX
    );

    vkResetFences(
        device.device(),
        1,
        &fence
    );
}

void ComputeContext::bufferBarrier(
    VkCommandBuffer cmd,
    Buffer&         buffer,
    BarrierType     type
) {
    VkBufferMemoryBarrier barrier{};
    VkPipelineStageFlags  srcStage = 0;
    VkPipelineStageFlags  dstStage = 0;

    switch (type) {
        case BarrierType::TransferToCompute:
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            srcStage              = VK_PIPELINE_STAGE_TRANSFER_BIT;
            dstStage              = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            break;

        case BarrierType::ComputeToCompute:
            barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            srcStage              = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            dstStage              = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            break;

        case BarrierType::ComputeToTransfer:
            barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            srcStage              = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            dstStage              = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;
    }

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;

    barrier.buffer = buffer.getBuffer();
    barrier.offset = 0;
    barrier.size   = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(
        cmd,
        srcStage,
        dstStage,
        0,
        0,
        nullptr,
        1,
        &barrier,
        0,
        nullptr
    );
}

} // namespace Nova