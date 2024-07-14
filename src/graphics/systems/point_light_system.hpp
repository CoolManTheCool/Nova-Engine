#ifndef POINT_LIGHT_SYSTEM_HPP
#define POINT_LIGHT_SYSTEM_HPP

#include "device.hpp"
#include "point_light_object.hpp"
#include "pipeline.hpp"
#include "camera.hpp"
#include "frame_info.hpp"
#include "resources.hpp"
// std
#include <memory>
#include <vector>

namespace nova {
class PointLightSystem {
 public:
  PointLightSystem(nova_Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void update(FrameInfo &frameInfo, GlobalUBO &ubo);
  void render(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  nova_Device &device;

  std::unique_ptr<nova_PipeLine> pipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace nova (not lve)

#endif  // RENDER_SYSTEM_HPP