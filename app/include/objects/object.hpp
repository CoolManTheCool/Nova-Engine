#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "types.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace Nova {

enum /*NOVA_ENGINE_API*/ {
	OBJECT_TYPE_NULL,
	OBJECT_TYPE_MESH,
	OBJECT_TYPE_POINT_LIGHT,
	OBJECT_TYPE_CAMERA,

	OBJECT_TYPE_COUNT,
};

struct TransformComponent {
  glm::dvec3 translation{};
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  glm::mat4 mat4();
  glm::mat3 normalMatrix();
};

class Object {
public:
	Object() = default;

	virtual unsigned int getObjectType();
	virtual void update(float deltaTime);
	virtual void render(RenderData &renderData);
	// private:
	TransformComponent transform{};
};

typedef std::vector<std::shared_ptr<Object>> ObjectList;

} // namespace Nova

#endif