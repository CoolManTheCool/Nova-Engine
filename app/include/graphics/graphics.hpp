#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include "objects/object.hpp"
#include "utility/config.hpp"
#include "systems/system.hpp"

#define MAX_FRAME_TIME 1.0

namespace Nova {

class Window;
class Device;
class Renderer;

struct FrameCtx {
    uint64_t frameCount = 0;
    double   deltaTime  = 0;
    bool     running    = true;
};

class Graphics {
public:
    Graphics(const EngineConfig& engineConfig);
    ~Graphics();

    Graphics(const Graphics&)            = delete;
    Graphics& operator=(const Graphics&) = delete;

    template <typename T>
    void addSystem() {
        static_assert(std::is_base_of<System, T>::value, "T must derive from System");
        auto system = std::make_shared<T>(*window, *device, *renderer);
        system->init();
        systems.push_back(system);
    }

    void waitDeviceIdle();

    const FrameCtx* startFrame();
    void            endFrame();

private:
    std::unique_ptr<Window>   window;
    std::unique_ptr<Device>   device;
    std::unique_ptr<Renderer> renderer;

    std::vector<std::shared_ptr<System>> systems;

    /**
     * Should be good for around 4.06 trillion years at 144 FPS
     * Age of the universe around 13.8 billion years
     * This should last around 294 times the age of the universe
     */
    uint64_t frameCount = 0;

    std::chrono::_V2::system_clock::time_point oldTime = std::chrono::high_resolution_clock::now();
};

} // namespace Nova