#ifndef GAME_HPP
#define GAME_HPP

#include "window.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "pipeline.hpp"

#include <memory>
#include <vector>

namespace nova {
class Game {
public:
    Game();  // Constructor declaration
    ~Game();
    void run();  // Member function declaration

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    nova_window window;
    nova_Device device;
    nova_SwapChain SwapChain{device, window.getExtent()};
    std::unique_ptr<nova_PipeLine> pipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
};
} // namespace nova oh yeah baby... - Tabnine Ai coder????????????

#endif