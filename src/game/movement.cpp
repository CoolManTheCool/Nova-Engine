#include "movement.hpp"

#include <limits>

namespace nova {
void MovementController::moveInPlaneXZ(GLFWwindow* window, float dt, nova_Object& gameObject) {

    vec3 rotate{0};
    rotate.y += static_cast<float>(glfwGetKey(window, keys.lookRight) == GLFW_PRESS);
    rotate.y -= static_cast<float>(glfwGetKey(window, keys.lookLeft) == GLFW_PRESS);
    rotate.x += static_cast<float>(glfwGetKey(window, keys.lookUp) == GLFW_PRESS);
    rotate.x -= static_cast<float>(glfwGetKey(window, keys.lookDown) == GLFW_PRESS);

    if(dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);

    gameObject.transform.rotation.x = clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
    gameObject.transform.rotation.y = mod(gameObject.transform.rotation.y, two_pi<float>());

    float yaw = gameObject.transform.rotation.y;
    const vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const vec3 upDir{0.f, -1.f, 0.f};

    vec3 moveDir{0.f};

    moveDir += forwardDir * static_cast<float>(glfwGetKey(window, keys.moveForward) == GLFW_PRESS);
    moveDir -= forwardDir * static_cast<float>(glfwGetKey(window, keys.moveBackward) == GLFW_PRESS);
    moveDir += rightDir * static_cast<float>(glfwGetKey(window, keys.moveRight) == GLFW_PRESS);
    moveDir -= rightDir * static_cast<float>(glfwGetKey(window, keys.moveLeft) == GLFW_PRESS);
    moveDir += upDir * static_cast<float>(glfwGetKey(window, keys.moveUp) == GLFW_PRESS);
    moveDir -= upDir * static_cast<float>(glfwGetKey(window, keys.moveDown) == GLFW_PRESS);

    if(dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);

}
}