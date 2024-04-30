#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <filesystem> // for std::filesystem::current_path

std::string getExecutableDirectory();

struct Settings_t {
    // Window settings
    int width = 960;
    int height = 540;
    std::string title = "Nova Engine 0.3";
    // Shader Config:
    std::string vertFilepath = "/home/noah/github/Nova-Engine/src/resources/shaders/vertex.vert.spv";
    std::string fragFilepath = "/home/noah/github/Nova-Engine/src/resources/shaders/fragment.frag.spv";
};

// Declare Settings as extern
extern Settings_t Settings;

#endif // UTIL_HPP
