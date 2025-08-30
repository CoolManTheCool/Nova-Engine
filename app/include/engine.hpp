#ifndef ENGINE_HPP
#define ENGINE_HPP

// Forward facing

#include "graphics.hpp"
#include "resources.hpp"

#include <vector>
#include <memory>

#include "objects/object.hpp"

#define MAX_FRAME_TIME 1.f

namespace Nova {

class Engine {
public:
    Engine() = default;
    ~Engine();

    void init();

    void loop(const std::function<void(float)>& gameLogic);

    void setSettings(const Settings& settings);

    // Prevent copying
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

private:
    ObjectList objects;
    Settings settings;
    Resources resources;
    Graphics graphics{settings, resources};
};

} // namespace Nova

#endif // ENGINE_HPP