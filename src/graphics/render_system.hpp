#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "device.hpp"
#include "object.hpp"
#include "pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace nova {
class RenderSystem {
 public:
  RenderSystem(nova_Device &device, VkRenderPass renderPass);
  ~RenderSystem();

  RenderSystem(const RenderSystem &) = delete;
  RenderSystem &operator=(const RenderSystem &) = delete;

  void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<nova_Object> &gameObjects);

 private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  nova_Device &device;

  std::unique_ptr<nova_PipeLine> pipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace ~~lve~~nova

#endif  // RENDER_SYSTEM_HPP