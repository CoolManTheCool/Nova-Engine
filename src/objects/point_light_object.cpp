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
        //random color
        this->lightColor = hsv2rgb({(float) rand() / RAND_MAX * 360, 1, 1});
    }
};

unsigned int PointLightObject::getRenderType() {
    return RENDER_MODE_CIRCLE;
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

float index = 0;
float speed = 2.0f;
void PointLightObject::update(float deltaTime) {
    transform.translation = {
        sin(glfwGetTime() + index * speed) * 10,
        transform.translation.y,
        cos(glfwGetTime() + index * speed) * 10
    };
    index += 2*M_PI/5;
    //std::cout << "test: " << mod(index, (float)M_2_PI) << std::endl;
    if (index > 2*M_PI) index -= 2*M_PI;
}

}