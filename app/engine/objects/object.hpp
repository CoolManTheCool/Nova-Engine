#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "mesh.hpp"

#include <memory>

namespace nova {

enum {
	OBJECT_TYPE_NULL,
	OBJECT_TYPE_MESH,
	OBJECT_TYPE_POINT_LIGHT,
	OBJECT_TYPE_CAMERA,

	OBJECT_TYPE_COUNT,
};

struct TransformComponent {
  glm::dvec3 translation{};  // (position offset)
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  glm::mat4 mat4();
  glm::mat3 normalMatrix();
};

class nova_Object {
public:
	nova_Object() = default;

	// TransformComponent getTransform() const { return transform; }
    // vec3 getPosition() const { return transform.translation; };
    // void setPosition(const vec3 &position) { transform.translation = position; };
    // void setTransform(const TransformComponent _transform) { transform = _transform; };

	virtual unsigned int getObjectType();
	virtual void update(float deltaTime);
	virtual void render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer);
	// private:
	TransformComponent transform{};
};

} // namespace nova

#endif