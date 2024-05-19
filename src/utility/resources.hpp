#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "model.hpp"

class Resources_t {
public:
    Resources_t();
    nova::nova_Model* getModel(const std::string name) { throw ("Woops"); return (models.find(name)->second); }
    const std::string getShader(const std::string name) { return (shaderPaths.find(name)->second); };
    
private:
    std::map<std::string, std::string> shaderPaths;  // name, path
    std::map<std::string, nova::nova_Model*> models; // name, pointer to model
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