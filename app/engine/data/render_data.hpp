#ifndef RENDER_DATA_HPP
#define RENDER_DATA_HPP

#include <vulkan/vulkan.h>

namespace Nova {

struct RenderData {
    VkPipelineLayout &pipelineLayout;
    VkCommandBuffer &commandBuffer;
};

} // namespace Nova

#endif RENDER_DATA_HPP