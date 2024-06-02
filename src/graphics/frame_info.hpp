#ifndef FRAME_INFO_HPP
#define FRAME_INFO_HPP

#include "camera.hpp"
#include "mesh_object.hpp"

#include "vulkan/vulkan.h"

namespace nova {
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera &camera;
        VkDescriptorSet globalDescriptorSet;
        std::vector<std::shared_ptr<nova_Object>> &objects;

    };
}

#endif