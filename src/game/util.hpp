#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <filesystem> // for std::filesystem::current_path

std::string getExecutableDirectory();

struct Settings_t {
    // Window settings
    int width = 512;
    int height = 512;
    std::string title = "Unnamed: Nova Engine";
    // Shader Config:
    std::string vertFilepath = "/home/noah/github/Nova-Engine/src/resources/shaders/vertex.vert.spv";
    std::string fragFilepath = "/home/noah/github/Nova-Engine/src/resources/shaders/fragment.frag.spv";
};

// Declare Settings as extern
#ifndef SETTINGS_EXTERN
#define SETTINGS_EXTERN
extern Settings_t Settings;
#endif

#endif // UTIL_HPP
