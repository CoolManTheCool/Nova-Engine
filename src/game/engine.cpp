#include "engine.hpp"

#include "camera.hpp"
#include "movement.hpp"
#include "buffer.hpp"
#include "point_light_system.hpp"
#include "gui_system.hpp"

#include "resources.hpp"
#include "logger.hpp"

#define MAX_FRAME_TIME 1.f

#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>
#include <iostream>

using namespace glm;

namespace nova {

//using namespace nova_Logger;

Game::Game() {
	globalPool = nova_DescriptorPool::Builder(device)
	.setMaxSets(nova_SwapChain::MAX_FRAMES_IN_FLIGHT)
	.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nova_SwapChain::MAX_FRAMES_IN_FLIGHT)
	.build();

	loadGameObjects();


}

Game::~Game() {}

void Game::run() {
	std::vector<std::unique_ptr<nova_Buffer>> UBOBuffers(nova_SwapChain::MAX_FRAMES_IN_FLIGHT);
	for(size_t i = 0; i < UBOBuffers.size(); i++) {
		UBOBuffers[i] = std::make_unique<nova_Buffer>(device, sizeof(GlobalUBO), 1,
		 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		UBOBuffers[i]->map();
	}
	nova_Buffer globalUBOBuffer {
		device,
		sizeof(GlobalUBO),
		nova_SwapChain::MAX_FRAMES_IN_FLIGHT,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		device.properties.limits.minUniformBufferOffsetAlignment
	};
	globalUBOBuffer.map();

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

	MeshSystem renderSystem{device, Renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
	PointLightSystem pointLightSystem{device, Renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
  	Camera camera{};
    float aspect = Renderer.getAspectRation();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
  	camera.setViewTarget(vec3(-1.f, -2.f, 2.f), vec3(0.f, 0.f, 2.5f));

	auto viewerObject = nova_Object();
	viewerObject.transform.translation.z = -2.5;
	MovementController cameraController{};

	auto currentTime = std::chrono::high_resolution_clock::now();

	// Initalize ImGui
	GUI.Init_ImGui(&device, &window, &Renderer, &imguiPool);
	// Register Bindings,
	GUI.setBinding("Window Debug Closed", true);
    GUI.setBinding("Camera Speed", 1);
	GUI.setBinding("F3 Down", false);
	GUI.registerWindow([this]() {
		// GUI.getBindingValue<>("")
		if(glfwGetKey(window.getWindow(), GLFW_KEY_F3) == GLFW_PRESS && !GUI.getBindingValue<bool>("F3 Down")) {
			*GUI.getBindingPointer<bool>("Window Debug Closed") = !GUI.getBindingValue<bool>("Window Debug Closed");
			*GUI.getBindingPointer<bool>("F3 Down") = true;
		} else if(glfwGetKey(window.getWindow(), GLFW_KEY_F3) == GLFW_RELEASE) {
			*GUI.getBindingPointer<bool>("F3 Down") = false;
		}
		if (GUI.getBindingValue<bool>("Window Debug Closed") == true) {
			ImGui::Begin("Debug Window", GUI.getBindingPointer<bool>("Window Debug Closed"), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Test Text");
    		ImGui::SliderInt("Test Slider", GUI.getBindingPointer<int>("Camera Speed"), 0, 10);
    		ImGui::Text("Test Slider: %d", GUI.getBindingValue<int>("Camera Speed"));
    		ImGui::End();
		}	
	});
	// _window leaves scope.
	

  	while (!window.shouldClose()) {
    	glfwPollEvents();
		//nova_Logger::LogStream::log << "Camera Transform: " << viewerObject.transform.mat4();
		//nova_Logger::LogStream::log << "Screen Size: " << glm::vec2(static_cast<float>(Settings.width), static_cast<float>(Settings.height));
		//nova_Logger::Logger::log("hi");
		//nova_Logger::logstream << "hi";

		//Logger::Logger::log("Game Crashed!", Logger::CRITICAL, Logger::WHITE, Logger::RED);
		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;
		frameTime = glm::min(frameTime, 1.f);

		for(std::shared_ptr<nova_Object> &obj : Objects) {
			obj->update(frameTime);
		}

		cameraController.moveInPlaneXZ(window.getWindow(), frameTime * GUI.getBindingValue<int>("Camera Speed"), viewerObject);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    	if (window.wasWindowResized()) {
    		float aspect = Renderer.getAspectRation();
    		camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
    	}

		GUI.update();
    
		if (auto commandBuffer = Renderer.beginFrame()) {
			int frameIndex = Renderer.getFrameIndex();
			FrameInfo frameInfo{
				frameIndex,
				frameTime,
				commandBuffer,
				camera,
				globalDescriptorSets[frameIndex],
				Objects
			};
			GlobalUBO UBO{};
			
			UBO.projection = camera.getProjection();
			UBO.view = camera.getView();
			UBO.inverseView = camera.getInverseView();
			pointLightSystem.update(frameInfo, UBO);
			UBOBuffers[frameIndex]->writeToBuffer(&UBO);
			UBOBuffers[frameIndex]->flush();

      		Renderer.beginSwapChainRenderPass(commandBuffer);
      		renderSystem.render(frameInfo);
      		pointLightSystem.render(frameInfo);
			GUI.render(&commandBuffer);
      		Renderer.endSwapChainRenderPass(commandBuffer);
      		Renderer.endFrame();
  		}
	}

  	vkDeviceWaitIdle(device.device());

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	vkDestroyDescriptorPool(device.device(), imguiPool, NULL);

  	vkDeviceWaitIdle(device.device());
}

void Game::loadGameObjects() {
	{
  	auto obj = MeshObject();
  	obj.setModel(&device, Resources.getModel("smooth_vase"));
  	obj.transform.translation = {-.5f, 0.f, -1.5f};
  	obj.transform.scale = {5, -5, 5};
  	Objects.push_back(std::make_shared<MeshObject>(obj));

  	obj.setModel(&device, Resources.getModel("flat_vase"));
  	obj.transform.translation = {.5f, 1.f, -3.f};
  	obj.transform.scale = {5, -5, 5};
  	Objects.push_back(std::make_shared<MeshObject>(obj));

  	obj.setModel(&device, Resources.getModel("quad"));
  	obj.transform.translation = {0.01f, 0.f, 0.f};
  	obj.transform.scale = {15, 1, 15};
  	Objects.push_back(std::make_shared<MeshObject>(obj));
	}
	for(int i = 0; i < 5; i++) {
		auto obj = PointLightObject();
		//obj.lightColor = {0.0, 0.1, 0.1};
		obj.transform.translation = {0.0f, 2.f, 0.0f};
		obj.transform.scale.x = 1.0f;
		obj.lightIntensity = 2.2f;
		Objects.push_back(std::make_shared<PointLightObject>(obj));
	}
	
}


}  // namespace nova