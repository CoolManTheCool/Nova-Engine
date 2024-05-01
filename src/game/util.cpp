#include "util.hpp"

std::string getExecutableDirectory() {
    // Get the path of the executable
    std::filesystem::path executablePath = std::filesystem::canonical(std::filesystem::path("/proc/self/exe"));

    // Return the directory containing the executable
    return executablePath.parent_path().string();
}

Settings_t Settings = Settings_t();