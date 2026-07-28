#pragma once

#include "objects/object_ref.hpp"
#include "vulkan/vulkan.h"

namespace Nova {

struct FrameInfo {
    int               frameIndex;
    float             frameTime;
    VkCommandBuffer   commandBuffer;
    VkDescriptorSet   globalDescriptorSet;
    ObjectRef<Object> root;
};

} // namespace Nova