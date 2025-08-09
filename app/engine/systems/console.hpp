#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "gui_system.hpp"
#include "util.hpp"
#include "rollingbuffer.hpp"

#include <array>
#include <string>
#include <chrono>

namespace Nova {

class Console_T {
public:
    struct CommandContext {
        std::string commandName;
        std::string args;
    };

    typedef std::function<void(CommandContext)> CommandFunction;

    struct Command {
        const std::string& name;
        CommandFunction function;
    };

    struct console_element {
        std::string text;
        std::chrono::system_clock::time_point sent;
        std::vector<std::string> details;
        unsigned short int level;
    };

    enum {
        INFO,      // Useful information,              eg: "Game engine initialized successfully."
        DEBUG,     // Default, Developer information,  eg: "Rendering optimization applied to scene X."
        WARNING,   // Caught bug, usually mitigated, eg: "Missing texture in asset, default texture applied."
        ERROR,     // Not Fatal, likely dangerous,     eg: "Failed to load configuration file, using default settings."
        CRITICAL,  // Likely Fatal,                    eg: "Out of memory, system is unstable and will shut down."

        MAX_LOG_LEVEL // 5
    };

    Console_T();
    void RegisterBindings();
    void RegisterWindow(Window* window);

    bool registerCommand(std::string command, CommandFunction func) {
        commandMap[command] = func;
        return true;
    }

    void resize(size_t size) {
        elements.resize(size);
    }

    void log(std::string text, unsigned short int level = INFO) {
        console_element Element;
        Element.level = level;
        Element.text = text;
        Element.sent = std::chrono::system_clock::now();
        elements.add(Element);
    }
    
private:
    RollingBuffer elements
    {std::type_index(typeid(console_element)), 100};

    char* filter = new char[25]();
    
    std::unordered_map<std::string, CommandFunction> commandMap;
};

extern Nova::Console_T Console;

}


#endif