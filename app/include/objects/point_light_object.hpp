#ifndef POINT_LIGHT_OBJECT_HPP
#define POINT_LIGHT_OBJECT_HPP

#include "objects/object.hpp"

#include <glm/glm.hpp>

namespace Nova {

struct PointLightPushConstants {
	glm::vec4 position{};
	glm::vec4 color{};
	float radius;
};

class PointLightObject : public Object {
public:
	/**
	 * @brief Initializes a point light object with given intensity, radius, and color.
	 * @param intensity Light intensity (Float).
	 * @param radius Light radius (Float).
	 * @param lightColor Light color (GLM Vec3).
	 */
    PointLightObject(float intensity = 2.0f, float radius = 1.f, glm::vec3 lightColor = glm::vec3(0.f, 0.f, 0.f));

    /**
	 * @brief Gets the static identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
	static const char* getStaticObjectType() { return "Point Light Object"; }

	/**
	 * @brief Gets the identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
  	const char* getObjectType() const override { return getStaticObjectType(); }

    //void setIntensity(float intensity) { lightIntensity = intensity; }
    //void setColor(const vec4 &color) { lightColor = color; }

    //float getIntensity() const { return lightIntensity; }
    //const vec4 &getColor() const { return lightColor; }
    

//private:
    float lightIntensity = 1.0f;
    glm::vec3 lightColor = glm::vec3(1.f, 0.f, 0.f);

    // transform.translation
    // transform.rotation
    // transform.scale
};

} // namespace Nova

#endif // POINT_LIGHT_OBJECT_HPP