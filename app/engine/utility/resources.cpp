#include "resources.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

#include <iterator>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

std::vector<std::string> searchDirectory(const std::string& directory);
uint64_t constexpr mix(char m, uint64_t s) {
    return ((s<<7) + ~(s>>3)) + ~m;
}
 
uint64_t constexpr _hash(const char * m) {
    return (*m) ? mix(*m, _hash(m+1)) : 0;
}

std::string getExecutableDirectory() {    std::string executablePath;

    // Get the full path to the executable
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    executablePath = std::string(buffer);
#else
    char buffer[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (length != -1) {
        buffer[length] = '\0';
        executablePath = std::string(buffer);
    }
#endif
    // Use std::filesystem to get the parent path
    std::filesystem::path pathObj = std::filesystem::path(executablePath).parent_path();
    // Convert to string and remove the trailing slash/backslash
    std::string directory = pathObj.string();
    // Remove trailing slash or backslash if present
    if (!directory.empty() && (directory.back() == '/' || directory.back() == '\\')) {
        directory.pop_back();
    }

    return directory;
}

Resources::Resources() {
    std::cout << "================================\n";
    std::cout << "Nova Engine \n";
    std::cout << "================================\n";

    executablePath = getExecutableDirectory();
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
            //(ext == "frag" || ext == "vert") ? std::cout : std::cout << "Name: " << name << " Ext: " << ext << "\n";
            switch(_hash(ext.c_str())) {
            case _hash("spv"):
                //std::cout << "- Shader: " << entry << "\n";
                shaderPaths.insert(std::make_pair(name, entry));
                break;
            case _hash("obj"):
                //models.insert(std::make_pair(name, Nova::Mesh::createBuilderFromFile(entry)));
                break;
        }
        }
    }
    std::cout << "\nShaders:\n";
    for (const auto& entry : shaderPaths) {
        std::cout << " - " << entry.first << "\n";
    }
    std::cout << "\nModels:\n";
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

Nova::Mesh::Builder Resources::getModel(const std::string name) { 
    auto p = models.find(name);
    if(p != models.end()) {
        return p->second;
    } else {
        throw std::invalid_argument("Mesh not found: " + name);
    }
}

const std::string Resources::getShader(const std::string name) {
    auto p = shaderPaths.find(name);
    if(p != shaderPaths.end()) {
        return p->second;
    } else {
        throw std::invalid_argument("Shader not found: " + name);
    }
}