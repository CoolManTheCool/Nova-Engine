#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include "util.hpp" // Assuming util.hpp contains necessary GLM overloads

namespace nova {

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

enum Color {
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    DEFAULT = 39
};

} // namespace nova

#endif // LOGGER_HPP
