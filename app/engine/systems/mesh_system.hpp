#pragma once

// Private facing

#include "device.hpp"
#include "pipeline.hpp"
#include "camera.hpp"
#include "frame_info.hpp"
#include "resources.hpp"

// std
#include <memory>
#include <vector>

namespace Nova {

class MeshSystem : public Object {
 public:
  MeshSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, Resources& resources);
  ~MeshSystem();

  MeshSystem(const MeshSystem &) = delete;
  MeshSystem &operator=(const MeshSystem &) = delete;

  void render(FrameInfo &frameInfo);
  using Object::render; // To avoid hiding the base class render method

  unsigned int getObjectType() override;

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass, Resources& resources);

  Device& device;

  std::unique_ptr<Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace ~~lve~~nova