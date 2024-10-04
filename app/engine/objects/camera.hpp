#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "util.hpp"
#include "object.hpp"
#include "window.hpp"

namespace nova {
class Camera : public nova_Object{
public:
    Camera(nova_Window &window, float fovy, float aspect, float near, float far);
    void setPerspectiveProjection(float fovy, float aspect, float near, float far);

	void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, 1.f, 0.f});
	void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, 1.f, 0.f});

    const mat4& getProjection() const { return projectionMatrix; };
    const mat4& getView() const { return viewMatrix; };
    const mat4& getInverseView() const { return inverseViewMatrix; };
    void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

    void moveInPlaneXZ(nova_Window* window, float dt);

    unsigned int getRenderType() override;
  	void update(float deltaTime) override;
  	void render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) override;

    float movement_speed = 3;
    float rotation_speed = 1.5f;

private:
    mat4 projectionMatrix{1.f};
    mat4 viewMatrix{1.f};
    mat4 inverseViewMatrix{1.f};
    nova_Window *window;
};

} // namespace nova
#endif