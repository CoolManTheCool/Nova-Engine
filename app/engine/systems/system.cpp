#include "systems/system.hpp"

#include "graphics/window.hpp"
#include "graphics/vulkan/device.hpp"
#include "graphics/window.hpp"

namespace Nova {

System::System(Window& window, Device& device, Renderer& renderer) :
window(window), device(device), renderer(renderer) {}

void System::update(double /*deltaTime*/) {}

void System::render(RenderData& /*renderData*/) {}

}