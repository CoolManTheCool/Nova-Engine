#ifndef nova_Renderer_HPP
#define nova_Renderer_HPP

#include "device.hpp"
#include "mesh.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace Nova {
class nova_Renderer {
public:
	nova_Renderer(nova_Window &window, nova_Device &device);
	~nova_Renderer();

	nova_Renderer(const nova_Renderer &) = delete;
	nova_Renderer &operator=(const nova_Renderer &) = delete;

	VkRenderPass getSwapChainRenderPass() const { return SwapChain->getRenderPass(); }
	float getAspectRation() const { return SwapChain->extentAspectRatio();}
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

	nova_Window &window;
	nova_Device &device;
	std::unique_ptr<nova_SwapChain> SwapChain;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> renderFinishedSemaphores;

	uint32_t currentImageIndex;
  	int currentFrameIndex{0};
	bool isFrameStarted{false};
};
} // namespace Nova

#endif