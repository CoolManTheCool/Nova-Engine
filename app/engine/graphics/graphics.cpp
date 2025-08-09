#include "graphics.hpp"

#include "descriptors.hpp"
#include "renderer.hpp"
#include "frame_info.hpp"
#include "buffer.hpp"

#include "gui_system.hpp"
#include "editor.hpp"
#include "console.hpp"
#include "mesh_system.hpp"

// Uncategorized includes
#include "resources.hpp"

namespace Nova {

Graphics::Graphics(Settings& settings) : settings(settings) {

}

void Graphics::init() {
    Device& device = renderer->getDevice();
    Resources resources;

    renderer = new Renderer(settings);


    globalPool = DescriptorPool::Builder(device)
	.setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
	.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
	.build();

    UBOBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for(size_t i = 0; i < UBOBuffers.size(); i++) {
        UBOBuffers[i] = std::make_unique<Buffer>(
            device,
            sizeof(GlobalUBO),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );
        UBOBuffers[i]->map();
    }

    globalUBOBuffer = std::make_unique<Buffer>(
        device,
        sizeof(GlobalUBO),
        SwapChain::MAX_FRAMES_IN_FLIGHT,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        device.properties.limits.minUniformBufferOffsetAlignment
    );
    globalUBOBuffer->map();

    auto globalSetLayout = DescriptorSetLayout::Builder(device)
	.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_ALL_GRAPHICS)
	.build();

    std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
	for (size_t i = 0; i < globalDescriptorSets.size(); i++) {
		auto bufferInfo = UBOBuffers[i]-> descriptorInfo();
		DescriptorWriter(*globalSetLayout, *globalPool)
		.writeBuffer(0, &bufferInfo)
		.build(globalDescriptorSets[i]);
	}

    MeshSystem renderSystem {
        device,
        renderer->getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout(),
        Resources()
    };
}

Graphics::~Graphics() {
    Device& device = renderer->getDevice();

    vkDeviceWaitIdle(device.device());

    UBOBuffers.clear();
    globalUBOBuffer.reset();

    if (ImGui::GetCurrentContext() != nullptr) {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    globalPool.reset();

    delete renderer;
}

}  // namespace Nova