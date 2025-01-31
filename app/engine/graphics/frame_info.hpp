#ifndef FRAME_INFO_HPP
#define FRAME_INFO_HPP

#include "camera.hpp"
#include "mesh_object.hpp"

#include "vulkan/vulkan.h"

#define MAX_LIGHTS 100

struct PointLight {
    vec4 position{}; // ignore w
    vec4 color{}; // w is intensity
};

struct GlobalUBO {
	glm::mat4 projection{1.f};
	glm::mat4 view{1.f};
    glm::mat4 inverseView{1.f};
	glm::vec4 ambientLightColor{1.f, 1.f, 1.f, 0.1f};
    PointLight pointLights[MAX_LIGHTS]; // UPDATE THIS IN THE SHADERS
    int numLights;
};
/*
layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[10];
  int numLights;
} ubo;
*/

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