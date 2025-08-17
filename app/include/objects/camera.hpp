#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "util.hpp"
#include "object.hpp"
#include "window.hpp"

#include <glm/glm.hpp>

namespace Nova {
class Camera : public Object {
public:
	Camera(Window *window, float fovy, float aspect, float near, float far);
	void setPerspectiveProjection(float fovy, float aspect, float near, float far);

	void setViewDirection(glm::vec3 position, glm::vec3 direction);
	void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, 1.f, 0.f});

	const glm::mat4& getProjection() const { return projectionMatrix; };
	const glm::mat4& getView() const { return viewMatrix; };
	const glm::mat4& getInverseView() const { return inverseViewMatrix; };
	void setViewYXZ();

	void moveInPlaneXZ(float dt);

	unsigned int getObjectType() override;
	void update(float deltaTime) override;

	float movement_speed = 3;
	float rotation_speed = 1.5f;

private:
	glm::mat4 projectionMatrix{1.f};
	glm::mat4 viewMatrix{1.f};
	glm::mat4 inverseViewMatrix{1.f};
	Window *window;
};

} // namespace Nova
#endif