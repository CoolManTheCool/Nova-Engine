#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <fstream>
#include <functional>

#include "GLFW/glfw3.h"

#include "mesh.hpp"

uint64_t constexpr mix(char m, uint64_t s);
uint64_t constexpr _hash(const char * m);

enum KeyMappings {
    moveLeft = GLFW_KEY_A,
    moveRight = GLFW_KEY_D,
    moveForward = GLFW_KEY_W,
    moveBackward = GLFW_KEY_S,
    moveUp = GLFW_KEY_SPACE,
    moveDown = GLFW_KEY_LEFT_SHIFT,
    lookLeft = GLFW_KEY_LEFT,
    lookRight = GLFW_KEY_RIGHT,
    lookUp = GLFW_KEY_UP,
    lookDown = GLFW_KEY_DOWN,
};

class Resources {
public:
    Resources();
    Nova::Mesh::Builder getModel(const std::string name);
    const std::string getShader(const std::string name);
    const std::string getExecutablePath() const { return executablePath; }
    
private:
    std::unordered_map<std::string, std::string> shaderPaths;  // name, path
    std::unordered_map<std::string, Nova::Mesh::Builder> models; // name, pointer to model
    //std::vector<std::string> texturePaths;

    std::string executablePath;
    //std::string vertFilepath = resourcePath + "shaders/vertex.vert.spv";
    //std::string fragFilepath = resourcePath + "shaders/fragment.frag.spv";
};

#endif // RESOURCES_HPP