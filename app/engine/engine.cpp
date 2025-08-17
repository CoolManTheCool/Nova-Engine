#include "engine.hpp"

#include "objects/camera.hpp"

namespace Nova {

void Engine::init() {
    graphics.init(); // Lines up beautifully

    // Push back new camera
    auto camera = std::shared_ptr<Camera>(new Camera(nullptr, 45.0f, 512 / static_cast<float>(512), 0.1f, 100.0f));
    camera->transform.translation = glm::vec3(0.0f, 0.0f, 3.0f);
    camera->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    /*
    camera->setPerspectiveProjection(
        glm::radians(45.0f),
        512 / static_cast<float>(512),
        0.1f,
        100.0f
    );
    */
    objects.push_back(camera);
}

void Engine::loop(const std::function<void()>& gameLogic) {
    while (true) {
        
        gameLogic();
        
        graphics.renderFrame(objects, 0.1f);
    }
}

}	// namespace Nova