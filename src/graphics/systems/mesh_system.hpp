#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "device.hpp"
#include "pipeline.hpp"
#include "camera.hpp"
#include "frame_info.hpp"
#include "resources.hpp"

// std
#include <memory>
#include <vector>

namespace nova {

class MeshSystem {
 public:
  MeshSystem(nova_Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~MeshSystem();

  MeshSystem(const MeshSystem &) = delete;
  MeshSystem &operator=(const MeshSystem &) = delete;

  void render(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  nova_Device &device;

  std::unique_ptr<nova_PipeLine> pipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace ~~lve~~nova

#endif  // RENDER_SYSTEM_HPP