#include "graphics/graphics.hpp"

#include <iostream>

#include "graphics/window.hpp"
#include "graphics/vulkan/device.hpp"

#include "glm/glm.hpp"

namespace Nova {

Graphics::Graphics(const EngineConfig& c) {
    window   = std::make_unique<Window>(c);
    device   = std::make_unique<Device>(*window, c);
    //renderer = std::make_unique<Renderer>(*window, *device);
}

Graphics::~Graphics() = default;

void Graphics::waitDeviceIdle() {
    vkDeviceWaitIdle(device->device());
}

const FrameCtx* Graphics::startFrame() {
    FrameCtx* ctx = new FrameCtx;
    window->pollEvents();
    auto now = std::chrono::high_resolution_clock::now();
    ctx->deltaTime = glm::min(std::chrono::duration<double>(now - oldTime).count(), MAX_FRAME_TIME);
    oldTime = now;
    ctx->frameCount = frameCount++;
    ctx->running = !window->shouldClose();

    return ctx;
}

void Graphics::endFrame() {
    // I'm scared...

    /*
    auto commandBuffer = renderer->beginFrame();

    renderer->beginSwapChainRenderPass(commandBuffer);

    VkPipelineLayout dummyPipeline = VK_NULL_HANDLE;
    RenderData rD { dummyPipeline, commandBuffer };

    renderer->endSwapChainRenderPass(commandBuffer);
    renderer->endFrame();
    */
}

}