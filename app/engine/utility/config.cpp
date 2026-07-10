#include "utility/config.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

#include <iostream>
#include <iterator>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

namespace Nova {

/**
 * Promiscuous function (I was listening to Promiscuous at the moment lol)
 * Yeah AI wrote this WHOLE function and it's got pretty smart labmda for static caching
 * Kinda perfect honestly... Just waiting for a crossplatform solution like std::filesystem::getExecutableDir();
 * I'm hoping std::filesystem::current_path() isn't it...
 */
std::string getExecutableDirectory() {
    static const std::string directory = []() {
        std::string executablePath;

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

        std::filesystem::path pathObj =
            std::filesystem::path(executablePath).parent_path();

        std::string result = pathObj.string();

        if (!result.empty() &&
            (result.back() == '/' || result.back() == '\\')) {
            result.pop_back();
        }

        return result;
    }();

    return directory;
}

EngineConfig::EngineConfig() {
    executablePath = getExecutableDirectory();
}

const std::string& EngineConfig::getExecPath() const {
    return executablePath;
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

std::vector<std::string> EngineConfig::getModulePaths() {
    std::vector<std::string> modulePaths;
    std::string modulesDir = executablePath + "/modules";

    if(debug) std::cout << "\nSearching for modules in: " << modulesDir << std::endl;

    for (const auto& path : searchDirectory(modulesDir)) {
        std::string ext;
        size_t dotPos = path.find_last_of(".") + 1;
        ext = (dotPos != 0) ? path.substr(dotPos) : "";
        if (ext == "dll" || ext == "so" || ext == "dylib") {
            modulePaths.push_back(path);
            if (debug) std::cout << " - Module found: " << path << std::endl;
        }
    }

    if (debug) std::cout << std::endl;
    return modulePaths;
}

}