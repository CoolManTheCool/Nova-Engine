#pragma once

#include <glm/glm.hpp>

/**
 * @brief A component that defines an object's position, rotation, and scale in 3D space.
 * Includes methods to generate transformation matrices.
 * @property translation The position of the object in 3D space.
 * @property scale The scale of the object along each axis.
 * @property rotation The rotation of the object in Euler angles (radians).
 */
struct TransformComponent {
    glm::dvec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

  	/**
   	 * @brief Generates a 4x4 transformation matrix combining translation, rotation, and scale.
   	 */
  	glm::mat4 mat4();
	
  	/**
     * @brief Generates a 3x3 normal matrix for transforming normals correctly.
     */
  	glm::mat3 normalMatrix();
};