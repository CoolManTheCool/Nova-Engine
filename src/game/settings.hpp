#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <filesystem> // for std::filesystem::current_path

struct Settings_t {
    // Window settings
    int width = 960;
    int height = 540;
    std::string title = "Nova Engine";

    // Shader Config:
    // add current path to the shader paths
    std::string vertFilepath =  std::filesystem::current_path().c_str() + "/src/resources/shaders/fragment.vert.spv".c_str();
    std::string fragFilepath = "./src/resources/shaders/fragment.frag.spv";
};

// Declare Settings as extern
extern Settings_t Settings;

#endif // SETTINGS_HPP
