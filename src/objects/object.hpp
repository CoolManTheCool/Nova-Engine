#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "mesh.hpp"

#include <memory>

namespace nova {

enum {
	RENDER_MODE_NULL   = 0,
	RENDER_MODE_MESH   = 1,
	RENDER_MODE_CIRCLE = 2,

	RENDER_MODE_COUNT  = 3,
};

struct TransformComponent {
  glm::vec3 translation{};  // (position offset)
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

	virtual unsigned int getRenderType();
	virtual void update(float deltaTime);
	virtual void render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer);
	// private:
	TransformComponent transform{};
};

} // namespace nova

#endif