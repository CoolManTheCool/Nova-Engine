#include "graphics/graphics.hpp"

#include <iostream>

#include "graphics/window.hpp"
#include "graphics/vulkan/device.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/renderer.hpp"

#include "systems/gui_system.hpp"

#include "glm/glm.hpp"

namespace Nova {

Graphics::Graphics(const EngineConfig& c) {
    window   = std::make_unique<Window>(c);
    device   = std::make_unique<Device>(*window, c);
    renderer = std::make_unique<Renderer>(*window, *device);

    addSystem<GUI_System>();
}

Graphics::~Graphics() = default;

/*
template<typename T>
void Graphics::addSystem() {
    static_assert(std::is_base_of<System, T>::value, "T must derive from System");
    auto system = std::make_shared<T>(*window, *device, *renderer);
    system->init();
    systems.push_back(system);
}
*/

void Graphics::waitDeviceIdle() {
    vkDeviceWaitIdle(device->device());
}

const FrameCtx* Graphics::startFrame() {
    FrameCtx* ctx = new FrameCtx;
    window->pollEvents();
    auto now        = std::chrono::high_resolution_clock::now();
    ctx->deltaTime  = glm::min(std::chrono::duration<double>(now - oldTime).count(), MAX_FRAME_TIME);
    oldTime         = now;
    ctx->frameCount = frameCount++;
    ctx->running    = !window->shouldClose();

    for (std::shared_ptr<System>& system : systems) {
        system->update(ctx->deltaTime);
    }

    return ctx;
}

void Graphics::endFrame() {
    auto commandBuffer = renderer->beginFrame();

    renderer->beginSwapChainRenderPass(commandBuffer);

    VkPipelineLayout dummyPipeline = VK_NULL_HANDLE;
    RenderData       rD{dummyPipeline, commandBuffer};

    for (std::shared_ptr<System>& system : systems) {
        system->render(rD);
    }

    renderer->endSwapChainRenderPass(commandBuffer);
    renderer->endFrame();
}

} // namespace Nova