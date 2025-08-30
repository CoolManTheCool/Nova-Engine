#include "debug.hpp"

#include <iostream>

unsigned int Debugger_T::create() {
    unsigned int id = nextId++;
    objects[id] = std::stacktrace::current();

    return id;
}

void Debugger_T::destroy(unsigned int id) {
    objects.erase(id);
}

void Debugger_T::print() const {
    for (const auto& [id, trace] : objects) {
        std::cout << "Object ID: " << id << "\n";
        std::cout << trace << "\n";
    }
}

Debugger_T Debugger;