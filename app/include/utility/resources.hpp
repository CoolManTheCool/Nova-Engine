#pragma once

// Public Facing

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <fstream>
#include <functional>

#include "components/mesh.hpp"

#define DEBUG_RESOURCES false

namespace Nova {

uint64_t constexpr mix(char m, uint64_t s);
uint64_t constexpr _hash(const char * m);

/*
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
*/

class Resources {
public:
    Resources();
    void loadMeshs(Device& device);
    std::shared_ptr<Mesh> getMesh(const std::string name);
    std::vector<char> getShader(const std::string name);
    std::vector<std::string> getModulePaths();
    const std::string getExecutablePath() const { return executablePath; }

    void cleanup();
    
private:
    std::unordered_map<std::string, std::string> shaderPaths;  // name, path
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshs; // name, shared_ptr to mesh
    std::vector<std::pair<std::string, std::string>> meshQueue; // List of pairs to be processed, name and filepath 
    //std::vector<std::string> texturePaths;

    std::string executablePath;
    //std::string vertFilepath = resourcePath + "shaders/vertex.vert.spv";
    //std::string fragFilepath = resourcePath + "shaders/fragment.frag.spv";
};

}