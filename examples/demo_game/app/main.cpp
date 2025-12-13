#include <string>
#include <iostream>

#include "engine.hpp"
#include "objects/mesh_object.hpp"
#include "objects/point_light_object.hpp"
#include "utility/resources.hpp"

#include "FancyUtility.hpp"

int main() {

    FancyUtilityFunction();

    Nova::Engine engine = Nova::Engine();

    Nova::Settings settings;
    settings.width = 1280;
    settings.height = 720;
    settings.title = "Demo Game | Nova Engine";
    settings.console_lines = 100;
    settings.version_name = "Alpha 1.1";

    engine.setSettings(settings);

    engine.init();

    auto meshObj = std::shared_ptr<Nova::MeshObject>(new Nova::MeshObject());
    meshObj->transform.translation = glm::vec3(0.0f, 0.0f, 2.0f);
    meshObj->transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
    meshObj->setMesh(engine.getResources().getMesh("cube"));
    engine.getRoot()->addChild(meshObj);

    meshObj = std::shared_ptr<Nova::MeshObject>(new Nova::MeshObject());
    meshObj->transform.translation = glm::vec3(2.0f, 0.0f, 3.0f);
    meshObj->transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
    meshObj->setMesh(engine.getResources().getMesh("ngon"));
    engine.getRoot()->addChild(meshObj);

    meshObj = std::shared_ptr<Nova::MeshObject>(new Nova::MeshObject());
    meshObj->transform.translation = glm::vec3(0.0f, -1.0f, 0.0f);
    meshObj->transform.scale = glm::vec3(10.0f, 1.0f, 10.0f);
    meshObj->setMesh(engine.getResources().getMesh("quad"));
    engine.getRoot()->addChild(meshObj);

    auto pointLight = std::shared_ptr<Nova::PointLightObject>(new Nova::PointLightObject(2, 1, glm::vec3(1.f, 0.2f, 0.2f)));
    pointLight->transform.translation = glm::vec3(0.5f, 3.0f, 2.0f);
    engine.getRoot()->addChild(pointLight);

    engine.loop([&](float) {

    });

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