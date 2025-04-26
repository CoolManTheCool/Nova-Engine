#include <string>
#include <iostream>
#include <type_traits>

#include "engine.hpp"
#include "mesh_object.hpp"
#include "point_light_object.hpp"

template<typename T, typename = std::enable_if_t<std::is_base_of_v<nova::nova_Object, T>>>
std::shared_ptr<T> createObject(nova::Engine& engine) {
    engine.Objects.push_back(std::make_shared<T>(T()));

    return std::dynamic_pointer_cast<T>(engine.Objects.back());
}

void loadGameObjects(nova::Engine& engine) {
    auto obj = createObject<nova::MeshObject>(engine);
    std::cout << "After creating first MeshObject, # of Objects: " << engine.Objects.size() << std::endl;
    
    obj->setModel(engine.getDevice(), engine.resources.getModel("smooth_vase"));
    obj->transform.translation = {-.5f, 0.f, -1.5f};
    obj->transform.scale = {5, -5, 5};
    
    obj = createObject<nova::MeshObject>(engine);
    std::cout << "After creating second MeshObject, # of Objects: " << engine.Objects.size() << std::endl;
    
    obj->setModel(engine.getDevice(), engine.resources.getModel("flat_vase"));
    obj->transform.translation = {.5f, 1.f, -3.f};
    obj->transform.scale = {5, -5, 5};
    
    obj = createObject<nova::MeshObject>(engine);
    std::cout << "After creating third MeshObject, # of Objects: " << engine.Objects.size() << std::endl;
    
    obj->setModel(engine.getDevice(), engine.resources.getModel("quad"));
    obj->transform.translation = {0.01f, 0.f, 0.f};
    obj->transform.scale = {15, 1, 15};
    
    const int objectCount = 20;
    const float radius = 8;
    for (int i = 0; i < objectCount; i++) {
        auto lightObj = createObject<nova::PointLightObject>(engine);
        std::cout << "After creating PointLightObject " << i << ", # of Objects: " << engine.Objects.size() << std::endl;

        // Setup light properties
        lightObj->transform.translation = {
            glm::cos(glm::radians(i * 360.0f / objectCount)) * radius,
            4,
            glm::sin(glm::radians(i * 360.0f / objectCount)) * radius
        };
        lightObj->transform.scale = {2, 2, 2};
        lightObj->lightIntensity = 0.9f;
    }    
}

int main() {
    nova::EngineConfig config;

    config.settings.console_lines = 100;
    config.settings.height = 1280;
    config.settings.width = 720;
    config.settings.title = "Nova Engine Development Build";
    config.settings.version_ID = 1;
    config.settings.version_name = "Alpha 1";

    nova::Engine engine = nova::Engine(config);

    loadGameObjects(engine);

    // Begin Render Loop
    engine.run();

    return EXIT_SUCCESS;    
}

/**
 * General rundown of the engine:
 * #include "engine.hpp"
 * 
 * void main() {
 *  EngineConfig config;
 *  config.appname = "My Amazing game!"
 *  config.resolution = { 1920, 1080 };
 *  Engine engine = Engine(config);
 * 
 *  engine.run([]() {
 *      // Main Loop
 *  });
 * }
 */