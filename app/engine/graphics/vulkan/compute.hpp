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

    void pipelineBarrier(VkCommandBuffer cmd);

    void copyBuffer(
        VkCommandBuffer cmd,
        Buffer&         src,
        Buffer&         dst,
        VkDeviceSize    size
    );

    void clearBuffer(
        VkCommandBuffer cmd,
        Buffer&         buffer,
        uint32_t        value = 0
    );

    void submit(VkCommandBuffer commandBuffer);

    VkQueue getQueue() const {
        return queue;
    }

private:
    void createCommandPool();
    void insertComputeBarrier(VkCommandBuffer cmd);

    Device& device;

    VkFence fence = VK_NULL_HANDLE;

    VkQueue       queue       = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;
};

} // namespace Nova