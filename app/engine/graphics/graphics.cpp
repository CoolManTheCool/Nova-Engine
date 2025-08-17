#include "graphics.hpp"

#include "descriptors.hpp"
#include "renderer.hpp"
#include "frame_info.hpp"
#include "buffer.hpp"

#include "gui_system.hpp"
#include "editor.hpp"
#include "console.hpp"
#include "mesh_system.hpp"

#include "objects/point_light_object.hpp"

namespace Nova {

Graphics::Graphics(Settings& settings, Resources& resources) : settings(settings), resources(resources) {
    globalDescriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
}

void Graphics::init() {
    renderer = new Renderer(settings);

    Device& device = renderer->getDevice();

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
        resources
    };

    GUI.Init_ImGui(device, renderer->getWindow(), renderer, globalPool->getDescriptorPool());
}

void Graphics::renderFrame(ObjectList& objects, float frameTime) {
    if (auto commandBuffer = renderer->beginFrame()) {
		int frameIndex = renderer->getFrameIndex();

        std::shared_ptr<Camera> camera = nullptr;

        // TODO: NOT loop every object to find camera
        for (const auto& obj : objects) {
            if (obj->getObjectType() == OBJECT_TYPE_CAMERA) {
                camera = std::dynamic_pointer_cast<Camera>(obj);
                break;
            }
        }

		FrameInfo frameInfo{
			frameIndex,
			frameTime,
			commandBuffer,
			*camera,
			globalDescriptorSets[frameIndex],
			objects
		};
		GlobalUBO UBO{};
			
		UBO.projection = camera->getProjection();
		UBO.view = camera->getView();
		UBO.inverseView = camera->getInverseView();
		uint8_t numObjects = 0;
		for(auto& obj : objects) {
			if(obj->getObjectType() == OBJECT_TYPE_POINT_LIGHT) {
				auto lightObject = std::dynamic_pointer_cast<PointLightObject>(obj);
				UBO.pointLights[numObjects].color = glm::vec4(lightObject->lightColor, lightObject->lightIntensity);
				UBO.pointLights[numObjects].position = glm::vec4(lightObject->transform.translation, 0);
				numObjects++;
			}
		}
			
		UBO.numLights = numObjects;
		UBOBuffers[frameIndex]->writeToBuffer(&UBO);
		UBOBuffers[frameIndex]->flush();

		renderer->beginSwapChainRenderPass(commandBuffer);
		//renderSystem.render(frameInfo);
		GUI.render(&commandBuffer);
		renderer->endSwapChainRenderPass(commandBuffer);
		renderer->endFrame();
	}
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