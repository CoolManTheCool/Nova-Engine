#pragma once

#include "graphics/vulkan/buffer.hpp"

#include <vulkan/vulkan.h>

namespace Nova {

class Device;

class ComputeContext {
public:
    ComputeContext(Device& device);
    ~ComputeContext();

    ComputeContext(const ComputeContext&)            = delete;
    ComputeContext& operator=(const ComputeContext&) = delete;

    VkCommandBuffer begin();

    void copyBuffer(
        VkCommandBuffer cmd,
        Buffer&         src,
        Buffer&         dst,
        VkDeviceSize    size);

    void end(VkCommandBuffer commandBuffer);

    void submit(VkCommandBuffer commandBuffer);

    VkQueue getQueue() const {
        return queue;
    }

private:
    void createCommandPool();

    Device& device;

    VkFence fence = VK_NULL_HANDLE;

    VkQueue       queue       = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;
};

} // namespace Nova