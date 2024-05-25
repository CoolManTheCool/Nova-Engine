#include "resources.hpp"

#include <iterator>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Resources_t Resources = Resources_t();

std::vector<std::string> searchDirectory(const std::string& directory);
uint64_t constexpr mix(char m, uint64_t s) {
    return ((s<<7) + ~(s>>3)) + ~m;
}
 
uint64_t constexpr hash(const char * m) {
    return (*m) ? mix(*m,hash(m+1)) : 0;
}

Resources_t::Resources_t() {
    // std::vector<std::string> shaderPaths;
    // std::vector<std::string> modelPaths;
    // std::vector<nova::nova_Model*> models;

    //shaderPaths[0] = std::pair<std::string, std::string>("vertex", "resources/shaders/vertex.vert.spv");

    executablePath = std::filesystem::current_path().string();
    std::cout << "Executable path: " << executablePath << std::endl;
    std::cout << "\nResources:\n";
    for (const auto& entry : searchDirectory(executablePath)) {
        std::string ext;
        std::string name;
        size_t dotPos = entry.find_last_of(".") + 1;
        ext = (dotPos != 0) ? entry.substr(dotPos) : "";
        size_t slashPos = entry.find_last_of("/") + 1;
        name = (dotPos != std::string::npos) ? entry.substr(0, dotPos-1) : "";
        name = (dotPos != std::string::npos) ? name.substr(slashPos) : "";
        if (!(name == "" || ext == "")) {
            (ext == "frag" || ext == "vert") ? std::cout : std::cout << "Name: " << name << " Ext: " << ext << "\n";
            switch(hash(ext.c_str())) {
            case hash("spv"):
                //std::cout << "- Shader: " << entry << "\n";
                shaderPaths.insert(std::make_pair(name, entry));
                break;
            case hash("obj"):
                models.insert(std::make_pair(name, nova::nova_Model::createBuilderFromFile(entry)));
                break;
        }
        }
    }
    std::cout << "\nShaders:\n";
    for (const auto& entry : models) {
        std::cout << " - " << entry.first << "\n";
    }
}

std::vector<std::string> searchDirectory(const std::string& directory) {
    std::vector<std::string> files;

    // Check if the given directory exists and is a directory
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        return files; // Return empty vector if directory is invalid
    }

    // Iterate through the directory
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry.path())) {
            // If the entry is a directory, recurse into it
            std::vector<std::string> subDirFiles = searchDirectory(entry.path().string());
            // Append the files found in the subdirectory to the main list
            files.insert(files.end(), subDirFiles.begin(), subDirFiles.end());
        } else if (fs::is_regular_file(entry.path())) {
            // If the entry is a file, add it to the list
            files.push_back(entry.path().string());
        }
    }

    return files;
}