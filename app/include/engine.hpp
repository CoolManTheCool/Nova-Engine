#ifndef ENGINE_HPP
#define ENGINE_HPP

// Forward facing

#include "graphics.hpp"

#include <vector>
#include <memory>

#include "objects/object.hpp"

namespace Nova {

class Engine {
public:
    Engine() = default;
    ~Engine() = default;

    void init();

    void setSettings(const Settings& settings) {
        this->settings = settings;
    }

    // Prevent copying
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

private:
    ObjectList Objects;
    Settings settings;
    Graphics graphics{settings};
};

} // namespace Nova

#endif // ENGINE_HPP