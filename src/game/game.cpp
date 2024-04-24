#include "game.hpp"
#include "util.hpp"

#include <stdexcept>
#include <array>

Settings_t Settings;

namespace nova {
Game::Game() : device(window) {
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

Game::~Game() {
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void Game::run() {
    while(!window.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device.device());
}

void Game::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void Game::createPipeline() {
    auto pipelineConfig = nova_PipeLine::defaultPipelineConfigInfo(SwapChain.width(), SwapChain.height());
    pipelineConfig.renderPass = SwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<nova_PipeLine>(device, pipelineConfig);
    //pipeline(device, nova_PipeLine::defaultPipelineConfigInfo(Settings.width, Settings.height))
}

void Game::createCommandBuffers() {
    commandBuffers.resize(SwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }

    for (int i=0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = SwapChain.getRenderPass();
        renderPassInfo.framebuffer = SwapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = SwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 0.0f};
        clearValues[1].depthStencil = {0.1f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        pipeline->bind(commandBuffers[i]);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
        }
    }
}
void Game::drawFrame() {
    uint32_t imageIndex;
    auto result = SwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    result = SwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present Swap Chain Image");
    }
}

}