#ifndef FRAME_INFO_HPP
#define FRAME_INFO_HPP

#include "camera.hpp"
#include "mesh_object.hpp"

#include "vulkan/vulkan.h"

#define MAX_LIGHTS 10

struct PointLight {
    vec4 position{}; // ignore w
    vec4 color{}; // w is intensity
};

struct GlobalUBO {
	glm::mat4 projection{1.f};
	glm::mat4 view{1.f};
	glm::vec4 ambientLightColor{1.f, 1.f, 1.f, 0.02f};
	glm::vec3 lightPosition{-1};
	alignas(16) glm::vec4 lightColor{1};
    PointLight pointLights[MAX_LIGHTS];
    int numLights;
};

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