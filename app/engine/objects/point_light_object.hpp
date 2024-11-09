#ifndef POINT_LIGHT_OBJECT_HPP
#define POINT_LIGHT_OBJECT_HPP

#include "object.hpp"

namespace nova {

struct PointLightPushConstants {
	vec4 position{};
	vec4 color{};
	float radius;
};

class PointLightObject : public nova_Object {
public:
    PointLightObject(float intensity = 2.0f, float radius = 1.f, vec3 lightColor = vec3(0.f, 0.f, 0.f));
    unsigned int getObjectType() override;
    void render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) override;
    void update(float deltaTime) override;

    //void setIntensity(float intensity) { lightIntensity = intensity; }
    //void setColor(const vec4 &color) { lightColor = color; }

    //float getIntensity() const { return lightIntensity; }
    //const vec4 &getColor() const { return lightColor; }
    

//private:
    float lightIntensity = 1.0f;
    vec3 lightColor = vec3(1.f, 0.f, 0.f);

    // transform.translation
    // transform.rotation
    // transform.scale
};

} // namespace nova

#endif // POINT_LIGHT_OBJECT_HPP