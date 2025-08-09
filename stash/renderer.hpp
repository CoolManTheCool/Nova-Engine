#ifndef Renderer_HPP
#define Renderer_HPP

#include "device.hpp"
#include "mesh.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace Nova {
class Renderer {
public:
	Renderer(Window &window, Device& device);
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

  	int getFrameIndex() const {
    	assert(isFrameStarted && "Cannot get frame index when frame not in progress");
    	return currentFrameIndex;
  	}

  	VkCommandBuffer beginFrame();
  	void endFrame();
  	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void recreateSwapChain();

private:
	void createCommandBuffers();
  	void freeCommandBuffers();

	Window &window;
	Device& device;
	std::unique_ptr<SwapChain> swapChain;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> renderFinishedSemaphores;

	uint32_t currentImageIndex;
  	int currentFrameIndex{0};
	bool isFrameStarted{false};
};
} // namespace Nova

#endif