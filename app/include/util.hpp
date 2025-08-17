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

namespace Nova {

struct Settings {
    int width = 512;
    int height = 512;
    std::string title = "Untitled Project | Nova Engine";
    std::string version_name = "Unknown Version";
    unsigned int console_lines = 100;

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

/**
 * WARNING TO ALL DEVS BEYOND THIS POINT:
 * The following code was written by me (Noah) at 11:32 PM on Augaust 15, 2025
 * and is some of the most mind-bending C++ I've ever written.
 * If you break it, I will find you and I will hurt you.
 * 
 * This code is a variadic template function to combine hash values.
 * Whatever the FUCK that means. (Copilot suggested that)
 * 
 * Throwback to my freshman year of highschool, playing my show for Band.
 * 
 * DARK MAGIC BELOW - YOU ARE WARNED
 */

// Base case: do nothing
inline void hashCombine(std::size_t&) {}

// Variadic case
template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
    static_assert(std::is_default_constructible_v<std::hash<T>>,
                  "Type must be hashable with std::hash");
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hashCombine(seed, rest...);
}

/**
 * DARK MAGIC WARNING OVER
 */

} // namespace Nova