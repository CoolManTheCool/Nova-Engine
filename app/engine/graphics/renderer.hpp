#pragma once

// Private facing

#include "device.hpp"
#include "window.hpp"
#include "swap_chain.hpp"
#include "utility/util.hpp"

namespace Nova {

struct RenderData {
    VkPipelineLayout &pipelineLayout;
    VkCommandBuffer &commandBuffer;
};

class Renderer {
public:
	Renderer(Settings& settings);
	~Renderer();

	Renderer(const Renderer &) = delete;
	Renderer &operator=(const Renderer &) = delete;

	VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
	float getAspectRation() const { return swapChain->extentAspectRatio();}
  	bool isFrameInProgress() const { return isFrameStarted; }

  	VkCommandBuffer getCurrentCommandBuffer() const {
    	assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
    	return commandBuffers[currentFrameIndex];
  	}

  	void freeCommandBuffers();

  	int getFrameIndex() const {
    	assert(isFrameStarted && "Cannot get frame index when frame not in progress");
    	return currentFrameIndex;
  	}

  	VkCommandBuffer beginFrame();
  	void endFrame();
  	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void recreateSwapChain();

	Device& getDevice();
	Window& getWindow();

private:
	void createCommandBuffers();

	Settings settings;

	std::unique_ptr<SwapChain> swapChain;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	Window window;
	Device device;

	uint32_t currentImageIndex;
  	int currentFrameIndex{0};
	bool isFrameStarted{false};
};

} // namespace Nova