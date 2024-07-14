#include "point_light_object.hpp"

namespace nova {
    
PointLightObject::PointLightObject(float intensity, float radius, vec4 lightColor){
    this->lightIntensity    = intensity;
    this->lightColor        = lightColor;
    this->transform.scale.x = radius;
};

unsigned int PointLightObject::getRenderType() {
    return RENDER_MODE_CIRCLE;
}

void PointLightObject::render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) {
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

void PointLightObject::update(float deltaTime) {

}

}