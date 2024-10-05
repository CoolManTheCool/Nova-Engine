#include "point_light_system.hpp"

// libs
#include "glm/gtc/matrix_transform.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace nova {

PointLightSystem::PointLightSystem(nova_Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		: device{device} {
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
}

PointLightSystem::~PointLightSystem() {
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PointLightPushConstants);

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

void PointLightSystem::createPipeline(VkRenderPass renderPass) {
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipelineConfig{};
	nova_PipeLine::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.attributeDescriptions.clear();
	pipelineConfig.bindingDescriptions.clear();
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<nova_PipeLine>(
			device,
			Resources.getShader("circle.vert"),
			Resources.getShader("circle.frag"),
			pipelineConfig);
}

void PointLightSystem::update(FrameInfo &frameInfo, GlobalUBO &ubo) {
	
	int lightIndex = 0;
	for (auto &obj : frameInfo.objects) {

		if (obj->getObjectType() != OBJECT_TYPE_POINT_LIGHT) continue;
		if (lightIndex+1 > MAX_LIGHTS) {
			std::cout << "[deprecated] Too many lights in scene.\n" 
			<< "Lights: " << lightIndex+1 << " Maximum: " << MAX_LIGHTS << std::endl;
			throw std::runtime_error("Scene has too many Light Objects, terminating!");
		};
		std::shared_ptr<PointLightObject> pointLight = std::dynamic_pointer_cast<PointLightObject>(obj);

		ubo.pointLights[lightIndex].position = glm::vec4(pointLight->transform.translation, 1.f);
		ubo.pointLights[lightIndex].color = glm::vec4(pointLight->lightColor, pointLight->lightIntensity);
		lightIndex++;
	}
	ubo.numLights = lightIndex;
}

void PointLightSystem::render(FrameInfo &frameInfo) {
	pipeline->bind(frameInfo.commandBuffer);

	//auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();

	vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);
	for (auto &obj : frameInfo.objects) {
		
		if(obj->getObjectType() != OBJECT_TYPE_POINT_LIGHT) continue;
		obj->render(pipelineLayout, frameInfo.commandBuffer);
	}
}

}  // namespace novaMORE T