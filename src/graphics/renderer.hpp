#ifndef nova_Renderer_HPP
#define nova_Renderer_HPP

#include "device.hpp"
#include "model.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace nova {
class nova_Renderer {
public:
	nova_Renderer(nova_window &window, nova_Device &device);
	~nova_Renderer();

	nova_Renderer(const nova_Renderer &) = delete;
	nova_Renderer &operator=(const nova_Renderer &) = delete;

	VkRenderPass getSwapChainRenderPass() const { return SwapChain->getRenderPass(); }
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

private:
	void createCommandBuffers();
  	void freeCommandBuffers();
	void recreateSwapChain();

	nova_window &window;
	nova_Device &device;
	std::unique_ptr<nova_SwapChain> SwapChain;
	std::vector<VkCommandBuffer> commandBuffers;

	uint32_t currentImageIndex;
  	int currentFrameIndex{0};
	bool isFrameStarted{false};
};
} // namespace nova

#endif