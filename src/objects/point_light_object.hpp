#ifndef POINT_LIGHT_OBJECT_HPP
#define POINT_LIGHT_OBJECT_HPP

#include "object.hpp"

namespace nova {
class PointLightObject : public nova_Object {
public:
    PointLightObject(float intensity = 2.0f, float radius = 1.f, vec4 lightColor = vec4(1.f, 0.f, 0.f, 1.f));
    unsigned int getRenderType() override;
    void render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) override;
    void update(float deltaTime) override;

    void setIntensity(float intensity) { lightIntensity = intensity; }
    void setColor(const vec4 &color) { lightColor = color; }
    void setPosition(const vec3 &position) { transform.translation = position; };
    void setTransform(const TransformComponent _transform) { transform = _transform; };

    float getIntensity() const { return lightIntensity; }
    const vec4 &getColor() const { return lightColor; }
    TransformComponent getTransform() const { return transform; }
    vec3 getPosition() const { return transform.translation; };

private:
    float lightIntensity = 1.0f;
    vec4 lightColor = vec4(1.f, 0.f, 0.f, 1.f);

    // transform.translation
    // transform.rotation
    // transform.scale
};

} // namespace nova

#endif // POINT_LIGHT_OBJECT_HPP