#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <fstream>
#include <functional>

#include "mesh.hpp"

uint64_t constexpr mix(char m, uint64_t s);
uint64_t constexpr _hash(const char * m);

class Resources_t {
public:
    Resources_t();
    nova::nova_Model::Builder getModel(const std::string name);
    const std::string getShader(const std::string name);
    
private:
    std::unordered_map<std::string, std::string> shaderPaths;  // name, path
    std::unordered_map<std::string, nova::nova_Model::Builder> models; // name, pointer to model
    //std::vector<std::string> texturePaths;

    std::string executablePath;// "/home/noah/Github/Nova-Engine/src/resources/";
    //std::string vertFilepath = resourcePath + "shaders/vertex.vert.spv";
    //std::string fragFilepath = resourcePath + "shaders/fragment.frag.spv";
};

#ifndef RESOURCES_EXTERN
#define RESOURCES_EXTERN
extern Resources_t Resources;
#endif

#endif // RESOURCES_HPP