#include "object.hpp"
#include "object_ref.hpp"

#include <iostream>
#include <stdexcept>

namespace Nova {

glm::mat4 TransformComponent::mat4() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    return glm::mat4{
        {
            scale.x * (c1 * c3 + s1 * s2 * s3),
            scale.x * (c2 * s3),
            scale.x * (c1 * s2 * s3 - c3 * s1),
            0.0f,
        },
        {
            scale.y * (c3 * s1 * s2 - c1 * s3),
            scale.y * (c2 * c3),
            scale.y * (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            scale.z * (c2 * s1),
            scale.z * (-s2),
            scale.z * (c1 * c2),
            0.0f,
        },
        {translation.x, translation.y, translation.z, 1.0f}};
}

glm::mat3 TransformComponent::normalMatrix() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);

    const glm::vec3 invScale = 1.0f / scale;

    return glm::mat3{
        {
            invScale.x * (c1 * c3 + s1 * s2 * s3),
            invScale.x * (c2 * s3),
            invScale.x * (c1 * s2 * s3 - c3 * s1),
        },
        {
            invScale.y * (c3 * s1 * s2 - c1 * s3),
            invScale.y * (c2 * c3),
            invScale.y * (c1 * c3 * s2 + s1 * s3),
        },
        {
            invScale.z * (c2 * s1),
            invScale.z * (-s2),
            invScale.z * (c1 * c2),
        }};
}

/// @brief Gets the indentifier for the render system that should render the given object.
/// @return unisgned int, 0 if not renderable.
unsigned int Object::getObjectType() {
    return OBJECT_TYPE_NULL;
}

void Object::update(float /*deltaTime*/) {};

void Object::render(RenderData&) {}

void Object::addChild(const std::shared_ptr<Object>& child) {
    if (!child) return;

    // Prevent duplicates
    if (std::find(children.begin(), children.end(), child) != children.end())
        return;

    // Remove from previous parent
    if (auto oldParent = child->parent.lock()) {
        oldParent->removeChild(child);
    }

    child->parent = shared_from_this();
    children.push_back(child);
}

void Object::removeChild(const std::shared_ptr<Object>& child) {
    if (!child) return;

    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        child->parent.reset();
        children.erase(it);
    }
}

void Object::clearChildren() {
    for (auto& c : children) {
        c->clearChildren();
        c->parent.reset();
    }
    children.clear();
}

void Object::removeFromParent() {
    if (auto p = parent.lock()) {
        p->removeChild(shared_from_this());
    }
    parent.reset();
}

void Object::queueFree() {
    removeFromParent();
    clearChildren();
}

ObjectRef<Object> Object::getParent() const {
    return ObjectRef<Object>(parent.lock());
}

std::vector<ObjectRef<Object>> Object::getChildren() const {
    std::vector<ObjectRef<Object>> refs;
    for (auto& c : children) {
        refs.emplace_back(c);
    }
    return refs;
}

std::vector<ObjectRef<Object>> Object::getChildrenRecursive() const {
    std::vector<ObjectRef<Object>> refs;
    for (auto& c : children) {
        auto childRefs = c->getChildrenRecursive();
        refs.insert(refs.end(), childRefs.begin(), childRefs.end());
        refs.emplace_back(c);
    }
    return refs;
}

Object::~Object() {
    clearChildren();
}

} // namespace Nova