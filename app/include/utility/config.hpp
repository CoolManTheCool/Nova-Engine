#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace Nova {

class EngineConfig /*startup config*/ {
public:
    EngineConfig();

    /**
     * Application Config
     */
    uint32_t width  = 1920/2;
    uint32_t height = 1080/2;
    
    // Version: Major.Minor.Patch
    uint32_t appMajorVer = 1;
    uint32_t appMinorVer = 0;
    uint32_t appPatchVer = 0;

    std::string title = "Untitled Application | Nova Engine";

    void* userData;

    // Enable VSync
    bool enableVSync = true;

    /**
     * Engine Config
     */

    // Version: Major.Minor.Patch
    const uint32_t EngMajorVer = 5;
    const uint32_t EngMinorVer = 1;
    const uint32_t EngPatchVer = 1;

    const char* engineName = "Nova Engine\0"; // Do NOT forget to null terminate

    const bool debug = false;

    const std::string& getExecPath() const;
    std::vector<std::string> getModulePaths();
private:
    std::string executablePath;

};

std::string getExecutableDirectory();

}
