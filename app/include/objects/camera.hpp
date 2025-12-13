#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "utility/util.hpp"
#include "objects/object.hpp"
#include "window.hpp"

#include <glm/glm.hpp>

namespace Nova {
class Graphics;

class Camera : public Object {
public:
	Camera(Graphics& graphics);
	/**
     * @brief Set the perspective projection matrix for the camera.
     * @param fovy Field of view in the y direction, in radians.
     * @param aspect Aspect ratio of the viewport (width / height).
     * @param near Near clipping plane distance.
     * @param far Far clipping plane distance.
     */
	void setPerspectiveProjection(float fovy, float aspect, float near, float far);

	/**
	 * @brief Move the camera to position and look in direction.
	 */
	void setViewDirection(glm::vec3 position, glm::vec3 direction);
	/**
  	 * @brief Move the camera to position and look toward target.
 	 */
	void setViewTarget(glm::vec3 position, glm::vec3 target);

	const glm::mat4& getProjection() const { return projectionMatrix; };
	const glm::mat4& getView() const { return viewMatrix; };
	const glm::mat4& getInverseView() const { return inverseViewMatrix; };
	void setViewYXZ();

	/**
     * @brief Basic fly flight WASD movement.
     */
	void moveInPlaneXZ(float dt);
	
	/**
	 * @brief Gets the static identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
	static const char* getStaticObjectType() { return "Camera Object"; }

	/**
	 * @brief Gets the identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
  	const char* getObjectType() const override { return getStaticObjectType(); }

	void update(float deltaTime) override;

	float movement_speed = 3;
	float rotation_speed = 1.5f;

private:
	glm::mat4 projectionMatrix{1.f};
	glm::mat4 viewMatrix{1.f};
	glm::mat4 inverseViewMatrix{1.f};
	Window& window;
};

} // namespace Nova
#endif