#include "renderer.hpp"

namespace Nova {

Renderer::Renderer(Settings& settings) : settings(settings), window(settings), device(window, settings) {
  recreateSwapChain();
  createCommandBuffers();
}

Renderer::~Renderer() {
  vkDeviceWaitIdle(device.device());
  freeCommandBuffers();
  swapChain.reset();
}

void Renderer::recreateSwapChain() {
    auto extent = window.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = window.getExtent();
        glfwWaitEvents();
    }

    // This is extremely cursed...
    // It'll probably be like this for another year or so...
    // It's 6:08 PM 7/28/25, a Monday after school.
    // I've been working on this stupid refactor for so long...
    // I just want to get this done so I can move on to the next thing.
    // I'm just thinking about all of the features I want to add,
    // and how much I want to improve the engine.
    // I want to add optimized rendering, physics, better everything.
    
    // Wait for all operations to complete
    vkDeviceWaitIdle(device.device());
    
    // Destroy old swapchain and create new one
    swapChain.reset();
    swapChain = std::make_unique<SwapChain>(device, extent);
}

Device& Renderer::getDevice() {
    return device;
}

Window& Renderer::getWindow() {
    return window;
}

void Renderer::createCommandBuffers() {
  commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }

  // Create semaphores for each swap chain image
  renderFinishedSemaphores.resize(swapChain->imageCount());
  
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  
  for (auto& semaphore : renderFinishedSemaphores) {
      if (vkCreateSemaphore(device.device(), &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
          throw std::runtime_error("failed to create synchronization objects for a frame!");
      }
  }
}

void Renderer::freeCommandBuffers() {
  vkFreeCommandBuffers(
      device.device(),
      device.getCommandPool(),
      static_cast<uint32_t>(commandBuffers.size()),
      commandBuffers.data());
  commandBuffers.clear();

  for (auto semaphore : renderFinishedSemaphores) {
      vkDestroySemaphore(device.device(), semaphore, nullptr);
  }
  renderFinishedSemaphores.clear();
}

VkCommandBuffer Renderer::beginFrame() {
  assert(!isFrameStarted && "Can't call beginFrame while already in progress");

  auto result = swapChain->acquireNextImage(&currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return nullptr;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  isFrameStarted = true;

  auto commandBuffer = getCurrentCommandBuffer();
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  return commandBuffer;
}

void Renderer::endFrame() {
  assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
  auto commandBuffer = getCurrentCommandBuffer();
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  // Use the semaphore corresponding to the current image
  VkSemaphore signalSemaphore = renderFinishedSemaphores[currentImageIndex];
  auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex, &signalSemaphore);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window.wasWindowResized()) {
    window.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }

  isFrameStarted = false;
  currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
  assert(
      commandBuffer == getCurrentCommandBuffer() &&
      "Can't begin render pass on command buffer from a different frame");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = swapChain->getRenderPass();
  renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
  viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
  assert(
      commandBuffer == getCurrentCommandBuffer() &&
      "Can't end render pass on command buffer from a different frame");
  vkCmdEndRenderPass(commandBuffer);
}

} // namespace Nova