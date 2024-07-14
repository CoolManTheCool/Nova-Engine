#include "mesh_system.hpp"

// libs
#include "glm/gtc/matrix_transform.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace nova {

MeshSystem::MeshSystem(nova_Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
    	: device{device} {
  	createPipelineLayout(globalSetLayout);
  	createPipeline(renderPass);
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

void MeshSystem::createPipeline(VkRenderPass renderPass) {
  	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  	PipelineConfigInfo pipelineConfig{};
  	nova_PipeLine::defaultPipelineConfigInfo(pipelineConfig);
  	pipelineConfig.renderPass = renderPass;
  	pipelineConfig.pipelineLayout = pipelineLayout;
  	pipeline = std::make_unique<nova_PipeLine>(
      device,
      Resources.getShader("mesh.vert"),
      Resources.getShader("mesh.frag"),
      pipelineConfig);
}

void MeshSystem::render(FrameInfo &frameInfo) {
  	pipeline->bind(frameInfo.commandBuffer);

  	//auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();

  	vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

  	for (auto &obj : frameInfo.objects) {

		obj->render(frameInfo);

    	/* replaced by nova_Object's render(), overloaded by whatever class needs it.
		SimplePushConstantData push{};
    	push.modelMatrix  = mesh->transform.mat4();
    	push.normalMatrix = mesh->transform.normalMatrix();
    	vkCmdPushConstants(
        	frameInfo.commandBuffer,
        	pipelineLayout,
        	VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        	0,
        	sizeof(SimplePushConstantData),
        	&push);
    	mesh->model->bind(frameInfo.commandBuffer);
    	mesh->model->draw(frameInfo.commandBuffer);
		*/
	}
}

}  // namespace nova