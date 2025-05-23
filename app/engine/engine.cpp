#include "engine.hpp"

#include "camera.hpp"
#include "buffer.hpp"
#include "gui_system.hpp"
#include "point_light_object.hpp"

#include "resources.hpp"
#include "util.hpp"
#include "editor.hpp"
#include "console.hpp"

#define MAX_FRAME_TIME 1.f

#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>
#include <iostream>

using namespace glm;

namespace nova {

//using namespace nova_Logger;

Engine::Engine(EngineConfig* config) :
window(config->settings),
device(window, config->settings),
Renderer(window, device) {

	srand(time(NULL));

	settings = config->settings;
	resources = &config->resources;

	globalPool = nova_DescriptorPool::Builder(device)
	.setMaxSets(nova_SwapChain::MAX_FRAMES_IN_FLIGHT)
	.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nova_SwapChain::MAX_FRAMES_IN_FLIGHT)
	.build();
}

Engine::~Engine() {
    vkDeviceWaitIdle(device.device());

    // Clear scene objects first since they may hold references to resources
    Objects.clear();

    // Clear buffers before destroying pools
    UBOBuffers.clear();
    globalUBOBuffer.reset();

    // Cleanup ImGui first
    if (ImGui::GetCurrentContext() != nullptr) {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // Destroy descriptor pools after ImGui cleanup
    if (imguiPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device.device(), imguiPool, nullptr);
        imguiPool = VK_NULL_HANDLE;
    }
    globalPool.reset();
}

void Engine::run() {
    // Initialize buffers as class members instead of local variables
    UBOBuffers.resize(nova_SwapChain::MAX_FRAMES_IN_FLIGHT);
    for(size_t i = 0; i < UBOBuffers.size(); i++) {
        UBOBuffers[i] = std::make_unique<nova_Buffer>(
            device,
            sizeof(GlobalUBO),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );
        UBOBuffers[i]->map();
    }

    globalUBOBuffer = std::make_unique<nova_Buffer>(
        device,
        sizeof(GlobalUBO),
        nova_SwapChain::MAX_FRAMES_IN_FLIGHT,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        device.properties.limits.minUniformBufferOffsetAlignment
    );
    globalUBOBuffer->map();

	auto globalSetLayout = nova_DescriptorSetLayout::Builder(device)
	.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_ALL_GRAPHICS)
	.build();

	std::vector<VkDescriptorSet> globalDescriptorSets(nova_SwapChain::MAX_FRAMES_IN_FLIGHT);
	for (size_t i = 0; i < globalDescriptorSets.size(); i++) {
		auto bufferInfo = UBOBuffers[i]-> descriptorInfo();
		nova_DescriptorWriter(*globalSetLayout, *globalPool)
		.writeBuffer(0, &bufferInfo)
		.build(globalDescriptorSets[i]);
	}

	MeshSystem renderSystem{device, Renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout(), resources};
	/*
	float aspect = Renderer.getAspectRation();
	camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
	camera.setViewTarget(vec3(-1.f, -2.f, 2.f), vec3(0.f, 0.f, 2.5f));
	*/

	Camera _camera{&window, glm::radians(65.f), Renderer.getAspectRation(), 0.1f, 1000.f };
	shared_ptr<Camera> camera = make_shared<Camera>(_camera);
	Objects.emplace_back(camera);
	camera->setViewDirection(vec3(-4.7, 2.6, -1.4), vec3(0.4, 1.7, 0));

	auto oldTime = std::chrono::high_resolution_clock::now();

	// Initalize ImGui
	GUI.Init_ImGui(&device, &window, &Renderer, &imguiPool);
	// Register Bindings,
	GUI.setBinding("Window Debug Open", true);
	GUI.setBinding("Camera Speed", 3.0f);
	GUI.setBinding("Camera FOV", radians(65.0f));
	GUI.setBinding("Debug Menu Key Down", false);
	GUI.setBinding("Objects", &Objects);
	 
	Editor_T Editor;
	Editor.RegisterBindings();
	Console.RegisterBindings();

	Editor.RegisterWindow(&window);
	Console.RegisterWindow(&window);

	GUI.registerWindow([this, camera]() {
		// GUI.getBindingValue<>("")
		if(glfwGetKey(window.getWindow(), GLFW_KEY_F3) == GLFW_PRESS && !GUI.getBindingValue<bool>("Debug Menu Key Down")) {
			*GUI.getBindingPointer<bool>("Window Debug Open") = !GUI.getBindingValue<bool>("Window Debug Open");
			*GUI.getBindingPointer<bool>("Debug Menu Key Down") = true;
		} else if(glfwGetKey(window.getWindow(), GLFW_KEY_F3) == GLFW_RELEASE) {
			*GUI.getBindingPointer<bool>("Debug Menu Key Down") = false;
		}
		if (GUI.getBindingValue<bool>("Window Debug Open") == true) {
			ImGui::Begin("Debug Window", GUI.getBindingPointer<bool>("Window Debug Open"), ImGuiWindowFlags_NoCollapse);
			ImGui::Checkbox("Editor Menu", GUI.getBindingPointer<bool>("Window Editor Open"));
			ImGui::SliderFloat("Camera Speed", GUI.getBindingPointer<float>("Camera Speed"), 2.0f, 6.0f);
			if (ImGui::SliderAngle("Camera FOV", GUI.getBindingPointer<float>("Camera FOV"), 50.0f, 100.0f)) {
				float aspect = Renderer.getAspectRation();
				camera->setPerspectiveProjection(GUI.getBindingValue<float>("Camera FOV"), aspect, 0.1f, 1000.f);
			}
			ImGui::Text(settings.title.c_str());
			ImGui::Text("Written by CoolManTheCool");
			ImGui::Text("Copyright © 2024");

			ImGui::End();
		}	
	});

	Console.log("Nova Engine Initialized!", Console.INFO);
	Console.log("Nova Engine Version: " + settings.version_name, Console.INFO);
	Console.log("Nova Engine Settings: " + std::to_string(settings.width) + "x" + std::to_string(settings.height), Console.INFO);
	Console.log("Nova Engine Console Lines: " + std::to_string(settings.console_lines), Console.INFO);

	while (!window.shouldClose()) {
		glfwPollEvents();
		
		//nova_Logger::LogStream::log << "Camera Transform: " << viewerObject.transform.mat4();
		//nova_Logger::LogStream::log << "Screen Size: " << glm::vec2(static_cast<float>(Settings.width), static_cast<float>(Settings.height));
		//nova_Logger::Logger::log("hi");
		//nova_Logger::logstream << "hi";

		//Logger::Logger::log("Game Crashed!", Logger::CRITICAL, Logger::WHITE, Logger::RED);
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - oldTime).count();
		oldTime = std::chrono::high_resolution_clock::now();
		frameTime = glm::min(frameTime, MAX_FRAME_TIME);

		for(std::shared_ptr<nova_Object> &obj : Objects) {
			obj->update(frameTime);
		}

		camera->movement_speed = GUI.getBindingValue<float>("Camera Speed");


		if (window.wasWindowResized()) {
			float aspect = Renderer.getAspectRation();
			camera->setPerspectiveProjection(GUI.getBindingValue<float>("Camera FOV"), aspect, 0.1f, 1000.f);
			window.resetWindowResizedFlag();
			Renderer.recreateSwapChain();
		}

		GUI.update();
	
		if (auto commandBuffer = Renderer.beginFrame()) {
			int frameIndex = Renderer.getFrameIndex();
			FrameInfo frameInfo{
				frameIndex,
				frameTime,
				commandBuffer,
				*camera,
				globalDescriptorSets[frameIndex],
				Objects
			};
			GlobalUBO UBO{};
			
			UBO.projection = camera->getProjection();
			UBO.view = camera->getView();
			UBO.inverseView = camera->getInverseView();
			uint8_t numObjects = 0;
			for(auto& obj : Objects) {
				if(obj->getObjectType() == OBJECT_TYPE_POINT_LIGHT) {
					auto lightObject = std::dynamic_pointer_cast<PointLightObject>(obj);
					UBO.pointLights[numObjects].color = vec4(lightObject->lightColor, lightObject->lightIntensity);
					UBO.pointLights[numObjects].position = vec4(lightObject->transform.translation, 0);
					numObjects++;
				}
			}
			
			UBO.numLights = numObjects;
			UBOBuffers[frameIndex]->writeToBuffer(&UBO);
			UBOBuffers[frameIndex]->flush();

			Renderer.beginSwapChainRenderPass(commandBuffer);
			renderSystem.render(frameInfo);
			GUI.render(&commandBuffer);
			Renderer.endSwapChainRenderPass(commandBuffer);
			Renderer.endFrame();
		}
	}

	vkDeviceWaitIdle(device.device());
}

glm::vec3 randomVec3(float min, float max) {
	return glm::vec3(
		randRange<float>(min, max), // Random x-component
		randRange<float>(min, max), // Random y-component
		randRange<float>(min, max)	// Random z-component
	);
}

}	// namespace nova