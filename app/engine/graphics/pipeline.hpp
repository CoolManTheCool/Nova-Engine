#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "device.hpp"
#include "utility/util.hpp"
#include "utility/resources.hpp"

// I have std
#include <string>
#include <vector>

namespace Nova {

struct PipelineConfigInfo {
	PipelineConfigInfo() = default;
	PipelineConfigInfo(const PipelineConfigInfo &) = delete;
	PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

	std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	std::vector<VkDynamicState> dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class Pipeline {
public:
	Pipeline(Device& device, Resources& resources, const PipelineConfigInfo &configInfo);
	~Pipeline();
    Pipeline(const Pipeline &) = delete;
    Pipeline operator=(const Pipeline &) = delete;

	void bind(VkCommandBuffer commandBuffer);
	static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

private:

	void createGraphicsPipeline(Resources& resources, const PipelineConfigInfo &configInfo);

	void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

	Device& device;
	VkPipeline graphicsPipeLine;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
};
} // namespace Nova

#endif