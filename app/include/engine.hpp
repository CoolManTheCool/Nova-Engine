#pragma once

#include <memory>
#include <vector>

#include "objects/scene.hpp"
#include "systems/system.hpp"
#include "graphics/graphics.hpp"
#include "primatives/engine_config.hpp"

/**
 * Nova Engine X
 */

namespace Nova {

class Engine {
public:
    /**
     * @brief Constructor for the Engine.
     *        Sets up Vulkan and GLFW contexts/devices but does not begin the main loop.
     */
    Engine(const EngineConfig& config);

    void run();

    void setScene(std::shared_ptr<Scene> obj);
    std::weak_ptr<Scene> getScene();

private:
    Graphics graphics;

    std::shared_ptr<Scene> scene = std::make_shared<Nova::Scene>();
    std::vector<std::unique_ptr<System>> systems;
};

}
