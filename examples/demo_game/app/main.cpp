#include <string>
#include <iostream>

#include "engine.hpp"

#include "game.hpp"

int main() {

    Nova::Engine engine = Nova::Engine();

    Settings settings;
    settings.width = 500;
    settings.height = 1000;
    settings.title = "Demo Game | Nova Engine";
    settings.console_lines = 100;
    settings.version_name = "Alpha 2";

    engine.setSettings(settings);

    engine.init();

    return EXIT_SUCCESS;    
}

/*
#include <engine.hpp>

int main(void) {
    Nova::EngineConfig config;

    config.settings.console_lines = 100;
    config.settings.height = 720;
    config.settings.width = 1280;
    config.settings.title = "Nova Engine Development Build";
    config.settings.version_name = "Alpha 1";

    Nova::Engine engine = Nova::Engine(config);

    // Load Game Logic
    auto obj = engine.createObject<Nova::MeshObject>();
    obj->setModel(engine.getDevice(), engine.resources.getModel("smooth_vase"));
    obj->transform.translation = {0.f, 0.f, -1.5f};
    obj->transform.scale = {5, 2, 5};

    while (!engine.shouldClose()) {
        // Do game logic
        double deltaTime = engine.getDeltaTime();
        obj->transform.translation.x += 0.01f * deltaTime;

    }

    return EXIT_SUCCESS;    
}

*/