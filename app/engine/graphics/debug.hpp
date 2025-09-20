#pragma once

#include <map>
#include <string>

#include <boost/stacktrace.hpp>

class Debugger_T {
public:
    unsigned int create();
    void destroy(unsigned int id);

    void print() const;
private:
    std::map<unsigned int, boost::stacktrace::stacktrace> objects;
    unsigned int nextId = 0;
};

extern Debugger_T Debugger;
