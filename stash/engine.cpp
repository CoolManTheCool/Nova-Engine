#include "engine.hpp"

#include "camera.hpp"
#include "buffer.hpp"
#include "point_light_object.hpp"
#include "device.hpp"
#include "resources.hpp"
#include "util.hpp"
#include "window.hpp"

#define MAX_FRAME_TIME 1.f

#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>
#include <iostream>

namespace Nova {

Engine::Engine() :
settings(Settings()),
window(settings),
device(window, settings) {

	srand(time(NULL));

	resources = new Resources();

}

void Engine::run() {
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

		LoopContext ctx(frameTime, Objects, *this);
		if (loop(ctx) == false) {
			Console.log("Game loop returned false, exiting...", Console.INFO);
			break;  // Exit the loop if the game loop returns false
		}

		for(std::shared_ptr<Object> &obj : Objects) {
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

}	// namespace Nova