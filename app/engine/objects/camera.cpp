#include <cassert>
#include <limits>

#include "camera.hpp"
#include "resources.hpp"

namespace nova {

void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
  assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
  const float tanHalfFovy = tan(fovy / 2.f);
  projectionMatrix = glm::mat4{0.0f};
  projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
  projectionMatrix[1][1] = -1.f / (tanHalfFovy);
  projectionMatrix[2][2] = far / (far - near);
  projectionMatrix[2][3] = 1.f;
  projectionMatrix[3][2] = -(far * near) / (far - near);
}

Camera::Camera(nova_Window &window, float fovy, float aspect, float near, float far) {
  this->window = &window;
  setPerspectiveProjection(fovy, aspect, near, far);
}

void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
  const glm::vec3 w{glm::normalize(direction)};
  const glm::vec3 u{glm::normalize(glm::cross(w, up))};
  const glm::vec3 v{glm::cross(w, u)};

  viewMatrix = glm::mat4{1.f};
  viewMatrix[0][0] = u.x;
  viewMatrix[1][0] = u.y;
  viewMatrix[2][0] = u.z;
  viewMatrix[0][1] = v.x;
  viewMatrix[1][1] = v.y;
  viewMatrix[2][1] = v.z;
  viewMatrix[0][2] = w.x;
  viewMatrix[1][2] = w.y;
  viewMatrix[2][2] = w.z;
  viewMatrix[3][0] = -glm::dot(u, position);
  viewMatrix[3][1] = -glm::dot(v, position);
  viewMatrix[3][2] = -glm::dot(w, position);

  inverseViewMatrix = glm::mat4{1.f};
  inverseViewMatrix[0][0] = u.x;
  inverseViewMatrix[0][1] = u.y;
  inverseViewMatrix[0][2] = u.z;
  inverseViewMatrix[1][0] = v.x;
  inverseViewMatrix[1][1] = v.y;
  inverseViewMatrix[1][2] = v.z;
  inverseViewMatrix[2][0] = w.x;
  inverseViewMatrix[2][1] = w.y;
  inverseViewMatrix[2][2] = w.z;
  inverseViewMatrix[3][0] = position.x;
  inverseViewMatrix[3][1] = position.y;
  inverseViewMatrix[3][2] = position.z;
}

void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
  setViewDirection(position, target - position, up);
}

// MOVEMENT

void Camera::moveInPlaneXZ(nova_Window* window, float dt) {

  vec3 rotate{0};
  
  rotate.y += static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::lookRight) == GLFW_PRESS);
  rotate.y -= static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::lookLeft) == GLFW_PRESS);
  rotate.x -= static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::lookUp) == GLFW_PRESS);
  rotate.x += static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::lookDown) == GLFW_PRESS);

  if(dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) transform.rotation += rotation_speed * dt * glm::normalize(rotate);

  transform.rotation.x = glm::clamp(transform.rotation.x, -1.5f, 1.5f);
  transform.rotation.y = mod(transform.rotation.y, two_pi<float>());

  float yaw = transform.rotation.y;
  const vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
  const vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
  const vec3 upDir{0.f, 1.f, 0.f};

  vec3 moveDir{0.f};

  moveDir += forwardDir * static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::moveForward) == GLFW_PRESS);
  moveDir -= forwardDir * static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::moveBackward) == GLFW_PRESS);
  moveDir += rightDir * static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::moveRight) == GLFW_PRESS);
  moveDir -= rightDir * static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::moveLeft) == GLFW_PRESS);
  moveDir += upDir * static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::moveUp) == GLFW_PRESS);
  moveDir -= upDir * static_cast<float>(glfwGetKey(window->getWindow(), KeyMappings::moveDown) == GLFW_PRESS);

  if(dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) transform.translation += movement_speed * dt * glm::normalize(moveDir);

}

/// @brief Usefull for debugging and knowing what object this is.
/// @return unisgned int
unsigned int Camera::getRenderType() {
    return OBJECT_TYPE_NULL;
}

void Camera::setViewYXZ(glm::vec3 position, glm::vec3 rotation) {
  const float c3 = glm::cos(rotation.z);
  const float s3 = glm::sin(rotation.z);
  const float c2 = glm::cos(rotation.x);
  const float s2 = glm::sin(rotation.x);
  const float c1 = glm::cos(rotation.y);
  const float s1 = glm::sin(rotation.y);
  const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
  const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
  const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
  viewMatrix = glm::mat4{1.f};
  viewMatrix[0][0] = u.x;
  viewMatrix[1][0] = u.y;
  viewMatrix[2][0] = u.z;
  viewMatrix[0][1] = v.x;
  viewMatrix[1][1] = v.y;
  viewMatrix[2][1] = v.z;
  viewMatrix[0][2] = w.x;
  viewMatrix[1][2] = w.y;
  viewMatrix[2][2] = w.z;
  viewMatrix[3][0] = -glm::dot(u, position);
  viewMatrix[3][1] = -glm::dot(v, position);
  viewMatrix[3][2] = -glm::dot(w, position);

  inverseViewMatrix = glm::mat4{1.f};
  inverseViewMatrix[0][0] = u.x;
  inverseViewMatrix[0][1] = u.y;
  inverseViewMatrix[0][2] = u.z;
  inverseViewMatrix[1][0] = v.x;
  inverseViewMatrix[1][1] = v.y;
  inverseViewMatrix[1][2] = v.z;
  inverseViewMatrix[2][0] = w.x;
  inverseViewMatrix[2][1] = w.y;
  inverseViewMatrix[2][2] = w.z;
  inverseViewMatrix[3][0] = position.x;
  inverseViewMatrix[3][1] = position.y;
  inverseViewMatrix[3][2] = position.z;
}

void Camera::update(float deltaTime) {
  /* do nothing for now...*/
}

void Camera::render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) {
  std::cout << "Camera Object should NOT be rendered." << std::endl;
  throw std::invalid_argument("render() called on Camera Object!");
}

}