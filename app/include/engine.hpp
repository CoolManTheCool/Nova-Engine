#pragma once

#include "graphics/graphics.hpp"

#include <vector>
#include <memory>

#include "objects/object.hpp"

namespace Nova {

class ModuleManager;

class Engine {
public:
    Engine(const EngineConfig& engineConfig);
    ~Engine();

    ObjectRef<Object> getRoot();

    template <typename T>
    void addSystem() {
        graphics.addSystem<T>();
    }

    using GameLogicFn = void (*)(const FrameCtx&, void* userData);
    void loop(GameLogicFn gameLogic);

    // Prevent copying
    Engine(const Engine&)            = delete;
    Engine& operator=(const Engine&) = delete;

private:
    std::shared_ptr<Object> root;
    Graphics                graphics;
    ModuleManager*          moduleManager;
    void*                   userData;
};

} // namespace Nova