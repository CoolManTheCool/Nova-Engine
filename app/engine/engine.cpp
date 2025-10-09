#include "engine.hpp"

#include "objects/camera.hpp"
#include "objects/mesh_object.hpp"
#include "renderer.hpp"
#include "gui_system.hpp"

// Public
#include "mesh_system.hpp"

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

#include <thread>
#include <memory> // Literally hate this library but I love it too

namespace Nova {

Engine::~Engine() {

    vkDeviceWaitIdle(graphics.getRenderer().getDevice().device());
    root.reset(); // Automatically clears children too
    resources.cleanup();
}

void Engine::init() {
    root = std::shared_ptr<Object>(new Object());

    root->addChild(graphics.init());

    auto camera = std::shared_ptr<Camera>(new Camera(graphics));
    camera->transform.translation = glm::vec3(0.0f, 0.0f, 3.0f);
    camera->transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    root->addChild(camera);
    camera->setPerspectiveProjection(
        glm::radians(60.0f),
        settings.width / static_cast<float>(settings.height),
        0.1f,
        100.0f
    );

    auto GUI = std::shared_ptr<GUI_System>(new GUI_System(graphics));
    root->addChild(GUI);
    
    GUI->setBinding("Window Debug Open", true);
	GUI->setBinding("Camera Speed", 3.0f);
	GUI->setBinding("Camera FOV", glm::radians(65.0f));
	GUI->setBinding("Debug Menu Key Down", false);
	GUI->setBinding("Objects", nullptr);

    GUI->registerWindow([this, &camera](GUI_System& GUI) {
		// GUI.getBindingValue<>("")
        Window& window = graphics.getRenderer().getWindow();
		if(glfwGetKey(window.getWindow(), GLFW_KEY_F3) == GLFW_PRESS && !GUI.getBindingValue<bool>("Debug Menu Key Down")) {
			*GUI.getBindingPointer<bool>("Window Debug Open") = !GUI.getBindingValue<bool>("Window Debug Open");
			*GUI.getBindingPointer<bool>("Debug Menu Key Down") = true;
		} else if(glfwGetKey(window.getWindow(), GLFW_KEY_F3) == GLFW_RELEASE) {
			*GUI.getBindingPointer<bool>("Debug Menu Key Down") = false;
		}
		if (GUI.getBindingValue<bool>("Window Debug Open") == true) {
			ImGui::Begin("Debug Window", GUI.getBindingPointer<bool>("Window Debug Open"), ImGuiWindowFlags_NoCollapse);
			//ImGui::Checkbox("Editor Menu", GUI.getBindingPointer<bool>("Window Editor Open"));
			if (ImGui::SliderFloat("Camera Speed", GUI.getBindingPointer<float>("Camera Speed"), 2.0f, 6.0f)) {
                camera->movement_speed = GUI.getBindingValue<float>("Camera Speed");
            }
			if (ImGui::SliderAngle("Camera FOV", GUI.getBindingPointer<float>("Camera FOV"), 50.0f, 100.0f)) {
				float aspect = graphics.getRenderer().getAspectRation();
				camera->setPerspectiveProjection(GUI.getBindingValue<float>("Camera FOV"), aspect, 0.1f, 1000.f);
			}
			ImGui::Text(settings.title.c_str());
			ImGui::Text("Written by CoolManTheCool");
			ImGui::Text("Copyright © 2024");

			ImGui::End();
		}	
	});
}

Resources& Engine::getResources() {
    return resources;
}

ObjectRef<Object> Engine::getRoot() {
    return ObjectRef<Object>(root);
}

void Engine::loop(const std::function<void(float)>& gameLogic) {
    auto oldTime = std::chrono::high_resolution_clock::now();

    unsigned long frameCount = 0;

    while (graphics.shouldClose() == false) {
        auto now = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float>(now - oldTime).count();
        oldTime = now;
        frameTime = glm::min(frameTime, MAX_FRAME_TIME);
        
        gameLogic(frameTime);

        for(ObjectRef<Object> &obj : root->getChildrenRecursive()) {
            obj->update(frameTime);
        }
        
        graphics.renderFrame(ObjectRef<Object>(root), frameTime);

        frameCount++;
    }
}

void Engine::setSettings(const Settings& settings) {
    this->settings = settings;
}

}	// namespace Nova