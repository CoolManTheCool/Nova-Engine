#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "gui_system.hpp"
#include "util.hpp"

#include <array>
#include <string>
#include <chrono>

#define MAX_CONSOLE_ELEMENTS 1000

namespace nova {

class Console_T {
public:
    struct console_element {
        std::string text;
        std::chrono::system_clock::time_point sent;
        unsigned short int level;
    };

    Console_T();
    void RegisterBindings();
    void RegisterWindow();
    
private:
    std::vector<console_element> elements;

    char* filter = new char[25]();
};

#ifndef CONSOLE_EXTERN
#define CONSOLE_EXTERN
extern Console_T Console;
#endif

}

#endif