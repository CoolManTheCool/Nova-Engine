#ifndef UTIL_HPP
#define UTIL_HPP
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

using namespace glm;

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec);
std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
std::ostream& operator<<(std::ostream& os, const glm::vec4& vec);
std::ostream& operator<<(std::ostream& os, const glm::mat2& mat);
std::ostream& operator<<(std::ostream& os, const glm::mat3& mat);
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);

struct Settings_t {
    Settings_t();
    // Window settings
    int width;
    int height;
    std::string title;
    std::string version_name;
    unsigned int version_ID;
    size_t console_lines;

    struct S_RendererSettings {
        S_RendererSettings();
        bool     ForceGPU;
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
namespace nova {
template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  (hashCombine(seed, rest), ...);
};
}

// Declare Settings as extern
#ifndef SETTINGS_EXTERN
#define SETTINGS_EXTERN
extern Settings_t Settings;
#endif

#endif // UTIL_HPP
