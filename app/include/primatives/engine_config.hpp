#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Nova {

/**
 * @brief ONLY for engine initalization.
 */
class EngineConfig {
public:
    EngineConfig();

    /**
     * Application Config
     */
    uint32_t width  = 960;
    uint32_t height = 540;
    
    // Version: Major.Minor.Patch
    uint32_t appMajorVer = 1;
    uint32_t appMinorVer = 0;
    uint32_t appPatchVer = 0;

    std::string formatAppVersion() const;

    std::string title = "Untitled Application | Nova Engine";

    /**
     * Engine Config
     */

    // Version: Major.Minor.Patch
    const uint32_t engMajorVer = 10;
    const uint32_t engMinorVer = 2;
    const uint32_t engPatchVer = 0;

    const std::string engineName = "Nova Engine";

    const std::string& getExecPath() const;
    std::vector<std::string> getModulePaths();

    /**
     * Enables validation layers right now and probably more later
     */
    bool debug = true;
private:
    std::string executablePath;

};

}
