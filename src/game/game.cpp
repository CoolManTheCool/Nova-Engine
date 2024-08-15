#include "game.hpp"

#include "mesh_system.hpp"
#include "camera.hpp"
#include "movement.hpp"
#include "buffer.hpp"
#include "point_light_system.hpp"

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
	//auto lightPosition = PointLightObject();

	//Initalize ImGUI
	init_imgui();

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

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Nova Engine");;
		// Imgui sampler
		bool button;
		ImGui::Checkbox("Checkbox", &button);
		ImGui::Text(button ? "Checkbox Enabled" : "Checkbox Disabled");
		ImGui::Text("Camera Position:");
		ImGui::Text(glm::to_string(viewerObject.transform.translation).c_str());
		ImGui::Text("Camera Rotation:");
		ImGui::Text(glm::to_string(viewerObject.transform.rotation).c_str());
		ImGui::Text("Hello, world!");
		float samples[100];
		for (int n = 0; n < 100; n++) {
			samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
		}
		ImGui::PlotLines("Samples", samples, 100);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

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
			ImGui::Render();
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
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

void Game::init_imgui() {
    // 1. Create a descriptor pool for ImGui
    VkDescriptorPoolSize poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = 100;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    
    if (vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &imguiPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool for ImGui!");
    }

    // 2. Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // or ImGui::StyleColorsClassic(), or ImGui::StyleColorsLight()

    // 3. Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForVulkan(window.getWindow(), true); // Pass your GLFW window

    // 4. Setup Vulkan binding
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device.getInstance();  // Your Vulkan instance
    init_info.PhysicalDevice = device.getPhysicalDevice();  // Your Vulkan physical device
    init_info.Device = device.device();  // Your Vulkan logical device
    init_info.QueueFamily = device.findPhysicalQueueFamilies().graphicsFamily;  // Queue family index
    init_info.Queue = device.graphicsQueue();  // Your Vulkan queue
    init_info.DescriptorPool = imguiPool;  // Descriptor pool for ImGui
    init_info.MinImageCount = nova_SwapChain::MAX_FRAMES_IN_FLIGHT;  // Min number of swapchain images
    init_info.ImageCount = nova_SwapChain::MAX_FRAMES_IN_FLIGHT;  // Number of swapchain images
    init_info.RenderPass = Renderer.getSwapChainRenderPass();  // Your Vulkan render pass

    ImGui_ImplVulkan_Init(&init_info);  // Pass the Vulkan render pass

    // 5. Upload Fonts
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device.getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device.device(), &allocInfo, &command_buffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &beginInfo);

    ImGui_ImplVulkan_CreateFontsTexture();

    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command_buffer;

    vkQueueSubmit(device.graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.graphicsQueue());

    vkFreeCommandBuffers(device.device(), device.getCommandPool(), 1, &command_buffer);

    // Destroy ImGui font texture
    ImGui_ImplVulkan_DestroyFontsTexture();
}


}  // namespace nova