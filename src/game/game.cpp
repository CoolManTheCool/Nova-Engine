#include "game.hpp"
#include "util.hpp"

#include <array>
#include <iostream>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#define ABSINE(x) abs(sin(x));

Settings_t Settings;

namespace nova {

struct SimplePushConstantData {
	glm::mat2 transform{1.f};
	glm::vec2 offset;
	//float time;
	alignas(16) glm::vec3 color;
};

Game::Game() : device(window) {
	loadGameObjects();
	createPipelineLayout();
	recreateSwapChain();
	createCommandBuffers();
	// std::cout << device.properties.limits.maxPushConstantsSize << "\n\n\n";
}

Game::~Game() { vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr); }

void Game::run() {
	// double framesElapsed = 0;
	double lastFrame = glfwGetTime();
	while (!window.shouldClose()) {
		// framesElapsed++;
		double delta = glfwGetTime() - lastFrame;
		//std::cout << "MS per frame: " << delta * 1000 << std::endl;
		glfwPollEvents();
		drawFrame();

		lastFrame = glfwGetTime();
	}
	vkDeviceWaitIdle(device.device());
}

void Game::loadGameObjects() {
	//reset random seed
	std::srand(std::time(NULL));
	std::vector<nova_Model::Vertex> vertices{
      {{0.0f, -0.5f}},
      {{0.5f, 0.5f}},
      {{-0.5f, 0.5f}}};

  for (int i = 0; i < 50; i++) {
	
  	auto triangle = nova_Object::createGameObject();
	triangle.setModel(&device, vertices);
  	triangle.color = {(float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX};
  	triangle.transform2d.scale = {(float)rand()/RAND_MAX/2, (float)rand()/RAND_MAX/2};
 	triangle.transform2d.rotation = .25f * M_PI * 2 + (float)rand()/RAND_MAX*2-1;
	triangle.transform2d.translation = {(float)rand()/RAND_MAX*2-1, (float)rand()/RAND_MAX*2-1};
	objects.push_back(std::move(triangle));
  }
}

void Game::createPipelineLayout() {

	VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void Game::createPipeline() {
	PipelineConfigInfo pipelineConfig{};
	nova_PipeLine::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = SwapChain->getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<nova_PipeLine>(device, pipelineConfig);
}

void Game::recreateSwapChain() {
	auto extent = window.getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device.device());
	SwapChain = nullptr;
	SwapChain = std::make_unique<nova_SwapChain>(device, extent);
	createPipeline();
}

void Game::createCommandBuffers() {
	commandBuffers.resize(SwapChain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}

void Game::recordCommandBuffer(int imageIndex) {
	VkCommandBufferBeginInfo beginInfo{};
  	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
    	throw std::runtime_error("failed to begin recording command buffer!");
  	}

  	VkRenderPassBeginInfo renderPassInfo{};
  	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  	renderPassInfo.renderPass = SwapChain->getRenderPass();
  	renderPassInfo.framebuffer = SwapChain->getFrameBuffer(imageIndex);

  	renderPassInfo.renderArea.offset = {0, 0};
  	renderPassInfo.renderArea.extent = SwapChain->getSwapChainExtent();

  	std::array<VkClearValue, 2> clearValues{};
  	clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  	clearValues[1].depthStencil = {1.0f, 0};
  	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  	renderPassInfo.pClearValues = clearValues.data();

  	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  	VkViewport viewport{};
  	viewport.x = 0.0f;
  	viewport.y = 0.0f;
  	viewport.width = static_cast<float>(SwapChain->getSwapChainExtent().width);
  	viewport.height = static_cast<float>(SwapChain->getSwapChainExtent().height);
  	viewport.minDepth = 0.0f;
  	viewport.maxDepth = 1.0f;
  	VkRect2D scissor{{0, 0}, SwapChain->getSwapChainExtent()};
  	vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  	vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  	renderGameObjects(commandBuffers[imageIndex]);

  	vkCmdEndRenderPass(commandBuffers[imageIndex]);
  	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    	throw std::runtime_error("failed to record command buffer!");
  	}
}

void Game::renderGameObjects(VkCommandBuffer commandBuffer) {
	pipeline->bind(commandBuffer);

  	for (auto& obj : objects) {
    	SimplePushConstantData push{};
		push.transform = obj.transform2d.mat2();
		push.offset = obj.transform2d.translation;
		push.color = obj.color;
    	vkCmdPushConstants(
        	commandBuffer,
        	pipelineLayout,
        	VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        	0,
        	sizeof(SimplePushConstantData),
        	&push);
    	obj.model->bind(commandBuffer);
    	obj.model->draw(commandBuffer);
  	}
}

void Game::drawFrame() {
	uint32_t imageIndex;
	auto result = SwapChain->acquireNextImage(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to acquire swap chain image!");
	}
	recordCommandBuffer(imageIndex);
	result = SwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()) {
		window.resetWindowResizedFlag();
		recreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present Swap Chain Image");
	}
}

} // namespace nova