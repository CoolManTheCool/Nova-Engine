#include "mesh_system.hpp"

// libs
#include "glm/gtc/matrix_transform.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace Nova {

MeshSystem::MeshSystem(nova_Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, Resources* resources)
    	: device{device} {
  	createPipelineLayout(globalSetLayout);
  	createPipeline(renderPass, resources);
}

MeshSystem::~MeshSystem() {
  	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void MeshSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
  	VkPushConstantRange pushConstantRange{};
  	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  	pushConstantRange.offset = 0;
  	pushConstantRange.size = sizeof(PushMeshData);

  	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

  	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
  	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
  	pipelineLayoutInfo.pushConstantRangeCount = 1;
  	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
      		VK_SUCCESS) {
    	throw std::runtime_error("failed to create pipeline layout!");
  }
}

void MeshSystem::createPipeline(VkRenderPass renderPass, Resources* resources) {
  	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  	PipelineConfigInfo pipelineConfig{};
  	nova_PipeLine::defaultPipelineConfigInfo(pipelineConfig);
  	pipelineConfig.renderPass = renderPass;
  	pipelineConfig.pipelineLayout = pipelineLayout;
  	pipeline = std::make_unique<nova_PipeLine>(
      device,
      resources->getShader("mesh.vert"),
      resources->getShader("mesh.frag"),
      pipelineConfig);
}

void MeshSystem::render(FrameInfo &frameInfo) {
  	pipeline->bind(frameInfo.commandBuffer);

  	//auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();

  	vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);
  	for (auto &obj : frameInfo.objects) {

		if(obj->getObjectType() != OBJECT_TYPE_MESH) continue;
		obj->render(pipelineLayout, frameInfo.commandBuffer);
		
	}
}

}  // namespace Nova