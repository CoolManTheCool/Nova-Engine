#include "point_light_object.hpp"

namespace nova {

vec3 hsv2rgb(vec3 in) {
    double hh, p, q, t, ff;
    long i;
    vec3 out;

    if (in.g <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.b;
        out.g = in.b;
        out.b = in.b;
        return out;
    }

    hh = in.r;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.b * (1.0 - in.g);
    q = in.b * (1.0 - (in.g * ff));
    t = in.b * (1.0 - (in.g * (1.0 - ff)));

    switch (i) {
    case 0:
        out.r = in.b;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.b;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.b;
        out.b = t;
        break;
    case 3:
        out.r = p;
        out.g = q;
        out.b = in.b;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.b;
        break;
    case 5:
    default:
        out.r = in.b;
        out.g = p;
        out.b = q;
        break;
    }

    return out;
}

PointLightObject::PointLightObject(float intensity, float radius, vec3 lightColor){
    this->lightIntensity    = intensity;
    this->transform.scale.x = radius;

    if(lightColor != vec3(0.0f, 0.0f, 0.0f)) { // If color is defined.
        this->lightColor = lightColor;

    } else { // If color is not defined.
        float randomHue = ((float) rand()) / RAND_MAX * 360.0f;
        float randomSaturation = ((float) rand()) / RAND_MAX * 0.7f + 0.3f; // Range: [0.3, 1.0]
        float randomValue = ((float) rand()) / RAND_MAX * 0.5f + 0.5f; // Range: [0.5, 1.0]
        
        this->lightColor = hsv2rgb({randomHue, randomSaturation, randomValue});
    }
};


unsigned int PointLightObject::getObjectType() {
    return OBJECT_TYPE_POINT_LIGHT;
}

void PointLightObject::render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) {
    //std::cout << "Rendering light object" << std::endl;
    PointLightPushConstants push{};
    push.position = glm::vec4(transform.translation, 1.f);
    push.color = glm::vec4(lightColor, lightIntensity);
    push.radius = transform.scale.x;

    vkCmdPushConstants(commandBuffer,
    pipelineLayout,
    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
    0,
    sizeof(PointLightPushConstants),
    &push);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

void PointLightObject::update(float deltaTime) {
    
}

}