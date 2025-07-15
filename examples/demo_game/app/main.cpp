#include <string>
#include <iostream>

#include "engine.hpp"

template<typename T, typename = std::enable_if_t<std::is_base_of_v<nova::nova_Object, T>>>
std::shared_ptr<T> createObject(nova::Engine& engine) {
    engine.Objects.push_back(std::make_shared<T>(T()));

    return std::dynamic_pointer_cast<T>(engine.Objects.back());
}

void loadGameObjects(nova::Engine& engine) {
    auto obj = createObject<nova::MeshObject>(engine);
    
    obj->setModel(engine.getDevice(), engine.resources->getModel("smooth_vase"));
    obj->transform.translation = {-.5f, 0.f, -1.5f};
    obj->transform.scale = {5, -5, 5};
    
    obj = createObject<nova::MeshObject>(engine);
    
    obj->setModel(engine.getDevice(), engine.resources->getModel("flat_vase"));
    obj->transform.translation = {.5f, 1.f, -3.f};
    obj->transform.scale = {5, -5, 5};
    
    obj = createObject<nova::MeshObject>(engine);
    
    obj->setModel(engine.getDevice(), engine.resources->getModel("quad"));
    obj->transform.translation = {0.01f, 0.f, 0.f};
    obj->transform.scale = {15, 1, 15};
}

int main() {
    nova::EngineConfig config;

    config.settings.console_lines = 100;
    config.settings.height = 720;
    config.settings.width = 1280;
    config.settings.title = "Nova Engine Demo Development Build";
    config.settings.version_name = "Alpha 1.1.3";

    nova::Engine engine = nova::Engine(&config);

    loadGameObjects(engine);

    nova::Console.registerCommand("help", []([[maybe_unused]] nova::Console_T::CommandContext context) {
        nova::Console.log("Available commands:", nova::Console.INFO);
        nova::Console.log("/help - Show this help message", nova::Console.INFO);
        nova::Console.log("/exit - Exit the application", nova::Console.INFO);
        nova::Console.log("/say [string] - Prints a message to console", nova::Console.INFO);
    });

    nova::Console.registerCommand("exit", [&engine]([[maybe_unused]] nova::Console_T::CommandContext context) {
        nova::Console.log("Exiting application by console command...", nova::Console.INFO);
        abort();
    });

    nova::Console.registerCommand("say", [&engine](nova::Console_T::CommandContext context) {
        if (context.args.empty()) {
            nova::Console.log("Usage: /say <message>", nova::Console.WARNING);
            return;
        }
        nova::Console.log("[Console] " + context.args, nova::Console.INFO);
    });

    std::cout << "This stuff is seriously working!!!" << std::endl;
    std::cout << "This stuff is seriously working!!!" << std::endl;
    std::cout << "This stuff is seriously working!!!" << std::endl;
    std::cout << "This stuff is seriously working!!!" << std::endl;
    std::cout << "This stuff is seriously working!!!" << std::endl;

    // Begin Render Loop
    engine.run();

    return EXIT_SUCCESS;    
}

/*
#include <engine.hpp>

int main(void) {
    nova::EngineConfig config;

    config.settings.console_lines = 100;
    config.settings.height = 720;
    config.settings.width = 1280;
    config.settings.title = "Nova Engine Development Build";
    config.settings.version_name = "Alpha 1";

    nova::Engine engine = nova::Engine(config);

    // Load Game Logic
    auto obj = engine.createObject<nova::MeshObject>();
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