#pragma once

// Public Facing

#include <string>
#include <filesystem> // for std::filesystem::current_path
#include <functional>
#include <iostream>
#include <type_traits>
#include <random>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec);
std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
std::ostream& operator<<(std::ostream& os, const glm::vec4& vec);
std::ostream& operator<<(std::ostream& os, const glm::mat2& mat);
std::ostream& operator<<(std::ostream& os, const glm::mat3& mat);
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);

struct Settings {
    int width = 512;
    int height = 512;
    std::string title = "Untitled Project | Nova Engine";
    std::string version_name = "Unknown Version";
    size_t console_lines = 100;

    struct RenderSettings {
        bool     ForceGPU = false;
        uint32_t ForceGPUID;
    } RenderSettings;
};

template <typename T>
T randRange(T min, T max) {
    static_assert(std::is_arithmetic<T>::value, "randRange requires numeric types.");

    // Seed with a real random value, if available
    std::random_device rd;
    std::mt19937 generator(rd());

    if constexpr (std::is_integral<T>::value) {
        // Integer version
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(generator);
    } else {
        // Floating-point version
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(generator);
    }
}

// Pretty damn close to dark magic
namespace Nova {
template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  (hashCombine(seed, rest), ...);
};
}