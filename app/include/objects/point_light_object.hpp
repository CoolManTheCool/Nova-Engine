#ifndef POINT_LIGHT_OBJECT_HPP
#define POINT_LIGHT_OBJECT_HPP

#include "object.hpp"

#include <glm/glm.hpp>

using namespace glm;

namespace Nova {

struct PointLightPushConstants {
	vec4 position{};
	vec4 color{};
	float radius;
};

class PointLightObject : public Object {
public:
    PointLightObject(float intensity = 2.0f, float radius = 1.f, vec3 lightColor = vec3(0.f, 0.f, 0.f));
    unsigned int getObjectType() override;

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

} // namespace Nova

#endif // POINT_LIGHT_OBJECT_HPP