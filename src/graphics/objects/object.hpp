#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "mesh.hpp"

#include <memory>

namespace nova {

struct TransformComponent {
  glm::vec3 translation{};  // (position offset)
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  glm::mat4 mat4();
  glm::mat3 normalMatrix();
};

class nova_Object {
  public:
	
	//nova_Object(const nova_Object &) = delete;
	//nova_Object &operator=(const nova_Object &) = delete;
	//nova_Object(nova_Object &&) = default;
	//nova_Object &operator=(nova_Object &&) = default;
	nova_Object() = default;

	virtual void render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) {}
	TransformComponent transform{};
};

} // namespace nova

#endif