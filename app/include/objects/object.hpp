#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "types.hpp"
#include "object_ref.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <algorithm>

namespace Nova {


enum /*NOVA_ENGINE_API*/ {
	OBJECT_TYPE_NULL,
	OBJECT_TYPE_MESH,
	OBJECT_TYPE_POINT_LIGHT,
	OBJECT_TYPE_CAMERA,
	SYSTEM_TYPE_MESH,
	SYSTEM_TYPE_GUI,

	OBJECT_TYPE_COUNT,
};

struct TransformComponent {
  glm::dvec3 translation{};
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  glm::mat4 mat4();
  glm::mat3 normalMatrix();
};

class Object;

typedef std::vector<std::shared_ptr<Object>> ObjectList;

class Object : public std::enable_shared_from_this<Object> {
public:
	Object() = default;
	~Object();

	virtual unsigned int getObjectType();
	virtual void update(float deltaTime);
	virtual void render(RenderData& renderData);

	// Child management
    void addChild(const std::shared_ptr<Object>& child);
    void removeChild(const std::shared_ptr<Object>& child);
    void clearChildren(); // recursive

	// Self removal (tbh a bit too real)
    void removeFromParent();
    void queueFree(); // deletes self and optionally children
 
	// Accessors
    ObjectRef<Object> getParent() const;
    std::vector<ObjectRef<Object>> getChildren() const;
    std::vector<ObjectRef<Object>> getChildrenRecursive() const;

	TransformComponent transform{};
private:
	std::weak_ptr<Object> parent;
	ObjectList children{};

};

} // namespace Nova

#endif