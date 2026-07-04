#include <iostream>

#include "engine.hpp"

int main() {
    Nova::EngineConfig c;

    c.title = "First Project | " + c.engineName + " " + c.formatAppVersion();
    c.appMajorVer = 1;
    c.appMajorVer = 0;
    c.appMajorVer = 0;

    Nova::Engine engine{c};

    auto scene = engine.getScene();

    engine.run();

    return 0;
}
