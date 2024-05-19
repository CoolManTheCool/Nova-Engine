#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <filesystem> // for std::filesystem::current_path
#include <functional>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    struct S_RendererSettings {
        S_RendererSettings();
        bool     ForceGPU;
        uint32_t ForceGPUID;
    } RenderSettings;
};



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
