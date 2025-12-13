#pragma once

#include "utility/types.hpp"
#include "objects/object_ref.hpp"
#include "utility/hash.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <algorithm>

namespace Nova {

/**
 * @brief A component that defines an object's position, rotation, and scale in 3D space.
 * Includes methods to generate transformation matrices.
 * @property translation The position of the object in 3D space.
 * @property scale The scale of the object along each axis.
 * @property rotation The rotation of the object in Euler angles (radians).
 * @method mat4 Generates a 4x4 transformation matrix combining translation, rotation, and scale.
 * @method normalMatrix Generates a 3x3 normal matrix for transforming normals correctly.
 */
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

	/**
	 * @brief Update logic, called once per frame.
	 * @param deltaTime Time elapsed since last frame in seconds.
	 */
	virtual void update(float deltaTime);

	/**
	 * @brief Gets the static identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
	static const char* getStaticObjectType() { return "Object"; }

	/**
	 * @brief Gets the identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
    virtual const char* getObjectType() const { return getStaticObjectType(); }

	/**
	 * @brief Render logic, called once per frame.
	 * Note that you cannot define this function in script.
	 * @param renderData Secret struct containing rendering context data.
	 */
	virtual void render(RenderData& renderData);

	// Child management
	/**
	 * @brief Adds a child object to this object.
	 * Note that I have no idea how you got a shared pointer to an object...
	 * @param child Shared pointer to the child object to add.
	 */
    void addChild(const std::shared_ptr<Object>& child);

	/** 
	 * @brief Removes a child object from this object.
	 * Note that this doesn't even make sense at all...
	 * @returns void early if child is null or not a child of this object.
	 * @param child Shared pointer to the child object to remove. (You don't have one? Too bad.)
	 */
    void removeChild(const std::shared_ptr<Object>& child);

	/**
	 * @brief Recursively clears all children from this object.
	 * This is the only reasonable function here...
	 */
    void clearChildren(); // recursive

	// Self removal (tbh a bit too real)
	/**
	 * @brief Removes this object from its parent, if it has one.
	 * Does nothing if there is no parent.
	 * Orphan
	 */
    void removeFromParent();

	/**
	 * @brief Queues this object for deletion by removing it from its parent and clearing its children.
	 * After calling this, there could be remaining ObjectRefs to this object, but they will be invalid.
	 * Thanks godot for inspiration
	 */
    void queueFree();
 
	// Accessors
	/**
	 * @brief Gets a reference to the parent object.
	 */
    ObjectRef<Object> getParent() const;

	/**
	 * @brief Get all of the childern.
	 * @todo Return array instead of vector?
	 * @return Vector of ObjectRefs to child objects.
	 */
    std::vector<ObjectRef<Object>> getChildren() const;
	
	/**
	 * @brief Get all children recursively.
	 * getChildren().getChildren().getChildren()...
	 * @return Vector of ObjectRefs to all descendant objects.
	 * @todo Return array instead.
	 */
    std::vector<ObjectRef<Object>> getChildrenRecursive() const;

	TransformComponent transform{};
private:
	std::weak_ptr<Object> parent;
	ObjectList children{};	
};

} // namespace Nova
