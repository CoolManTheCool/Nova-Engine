#pragma once

#include <map>
#include <string>
#include <stacktrace>

class Debugger_T {
public:
    unsigned int create();
    void destroy(unsigned int id);

    void print() const;
private:
    std::map<unsigned int, std::stacktrace> objects;
    unsigned int nextId;
};

extern Debugger_T Debugger;