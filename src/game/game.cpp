#include "game.hpp"

#include "mesh_system.hpp"
#include "point_light_system.hpp"
#include "camera.hpp"
#include "movement.hpp"
#include "buffer.hpp"

#include "resources.hpp"
#include "logger.hpp"

#define MAX_FRAME_TIME 1.f

#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>
#include <iostream>

using namespace glm;
struct GlobalUBO {
	glm::mat4 projection{1.f};
	glm::mat4 view{1.f};
	glm::vec4 ambientLightColor{1.f, 1.f, 1.f, 0.02f};
	glm::vec3 lightPosition{-1};
	alignas(16) glm::vec4 lightColor{1};
	//alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
};

namespace nova {

//using namespace nova_Logger;

Game::Game() {
	globalPool = nova_DescriptorPool::Builder(device)
	.setMaxSets(nova_SwapChain::MAX_FRAMES_IN_FLIGHT)
	.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nova_SwapChain::MAX_FRAMES_IN_FLIGHT)
	.build();

	loadGameObjects();

	//Resources.init(device);
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

	MeshSystem meshSystem{device, Renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
	PointLightSystem pointLightSystem{device, Renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
  	Camera camera{};
    float aspect = Renderer.getAspectRation();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
  	camera.setViewTarget(vec3(-1.f, -2.f, 2.f), vec3(0.f, 0.f, 2.5f));

	auto viewerObject = nova_Object();
	viewerObject.transform.translation.z = -2.5;
	MovementController cameraController{};

	auto currentTime = std::chrono::high_resolution_clock::now();
	auto lightPosition = nova_Object();
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
		frameTime = min(frameTime, MAX_FRAME_TIME);

		lightPosition.transform.translation = {
			sin(glfwGetTime()) * 3,
			1,
		 	cos(glfwGetTime()) * 3,
		};

		cameraController.moveInPlaneXZ(window.getWindow(), frameTime, viewerObject);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    	if (window.wasWindowResized()) {
    		float aspect = Renderer.getAspectRation();
    		camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
    	}
    
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

			// Get Lights from Objects.
			
			GlobalUBO UBO{};
			UBO.lightPosition = lightPosition.transform.translation;
			UBO.projection = camera.getProjection();
			UBO.view = camera.getView();
			UBOBuffers[frameIndex]->writeToBuffer(&UBO);
			UBOBuffers[frameIndex]->flush();
      		Renderer.beginSwapChainRenderPass(commandBuffer);
      		meshSystem.render(frameInfo);
      		pointLightSystem.render(frameInfo);
      		Renderer.endSwapChainRenderPass(commandBuffer);
      		Renderer.endFrame();
  		}
	}
  	vkDeviceWaitIdle(device.device());
}

void Game::loadGameObjects() {
  	auto obj = MeshObject();
  	obj.setModel(&device, Resources.getModel("flat_vase"));
  	obj.transform.translation = {-.5f, 0.f, -1.5f};
  	obj.transform.scale = {5, -5, 5};
  	Objects.push_back(std::move(std::make_shared<MeshObject>(obj)));

  	obj.setModel(&device, Resources.getModel("smooth_vase"));
  	obj.transform.translation = {.5f, 1.f, -3.f};
  	obj.transform.scale = {5, -5, 5};
  	Objects.push_back(std::move(std::make_shared<MeshObject>(obj)));

  	obj.setModel(&device, Resources.getModel("quad"));
  	obj.transform.translation = {0.01f, 0.f, 0.f};
  	obj.transform.scale = {5, 1, 5};
  	Objects.push_back(std::move(std::make_shared<MeshObject>(obj)));

	auto light = PointLightObject();
	light.lightColor = {1.f, 0.f, 0.f, 1.f};
	light.lightIntensity = 5;
	std::shared_ptr<PointLightObject> pointLightObject = std::make_shared<PointLightObject>(light);

    // Push back the shared pointer to the vector
    Objects.push_back(pointLightObject);
}

}  // namespace nova