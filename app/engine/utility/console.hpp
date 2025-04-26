#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "gui_system.hpp"
#include "util.hpp"

#include <array>
#include <string>
#include <chrono>

namespace nova {

class Console_T {
public:
    struct console_element {
        std::string text;
        std::chrono::system_clock::time_point sent;
        std::vector<std::string> details;
        unsigned short int level;
        enum {
            INFO,      // Useful information,              eg: "Game engine initialized successfully."
            DEBUG,     // Default, Developer information,  eg: "Rendering optimization applied to scene X."
            WARNING,   // Uncaught bug, usually mitigated, eg: "Missing texture in asset, default texture applied."
            ERROR,     // Not Fatal, likely dangerous,     eg: "Failed to load configuration file, using default settings."
            CRITICAL,  // Likely Fatal,                    eg: "Out of memory, system is unstable and will shut down."

            MAX_LOG_LEVEL // 5
        };
    };

    Console_T(Settings settings);
    void RegisterBindings();
    void RegisterWindow(nova_Window* window);
    
private:
    std::vector<console_element> elements;

    char* filter = new char[25]();
};

}

#endif