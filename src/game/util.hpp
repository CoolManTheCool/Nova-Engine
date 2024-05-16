#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <iostream>
#include <filesystem> // for std::filesystem::current_path
#include <functional>

std::string getExecutableDirectory();

struct Settings_t {
    // Window settings
    int width         = 512;
    int height        = 512;
    std::string title = "Unnamed: Nova Engine";

    struct S_RendererSettings {
        bool     ForceGPU   = false;
        uint32_t ForceGPUID = 7298; //0 // Force if # != null; My GPU ID
    } RenderSettings;

    // Resources Config:
    struct S_Resources_t {
        std::string resourcePath = "/home/noah/Github/Nova-Engine/src/resources/";

        std::string vertFilepath = resourcePath + "shaders/vertex.vert.spv";
        std::string fragFilepath = resourcePath + "shaders/fragment.frag.spv";

        std::string models[4] = {
            resourcePath + "models/colored_cube.obj",
            resourcePath + "models/cube.obj",
            resourcePath + "models/flat_vase.obj",
            resourcePath + "models/smooth_vase.obj",
        };
    } Resources;
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
