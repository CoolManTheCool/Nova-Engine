#ifndef GAME_HPP
#define GAME_HPP

#include "device.hpp"
#include "model.hpp"
#include "object.hpp"
#include "pipeline.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <memory>
#include <vector>

namespace nova {
class Game {
  public:
	Game(); // Constructor declaration
	~Game();
	void run(); // Member function declaration

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

  private:
	void loadGameObjects();
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void drawFrame();
	void recreateSwapChain();
	void recordCommandBuffer(int imageIndex);
	void renderGameObjects(VkCommandBuffer commandBuffer);

	nova_window window;
	nova_Device device;
	std::unique_ptr<nova_SwapChain> SwapChain;
	std::unique_ptr<nova_PipeLine> pipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<nova_Object> objects;
};
} // namespace nova

#endif