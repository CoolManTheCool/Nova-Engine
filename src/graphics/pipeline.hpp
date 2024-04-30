#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "device.hpp"
#include "util.hpp"

// I have std
#include <string>
#include <vector>

namespace nova {

struct PipelineConfigInfo {
	PipelineConfigInfo() = default;
	PipelineConfigInfo(const PipelineConfigInfo &) = delete;
	PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

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

class nova_PipeLine {
  public:
	nova_PipeLine(nova_Device &device, const PipelineConfigInfo &configInfo);
	~nova_PipeLine();
	nova_PipeLine(const nova_PipeLine &) = delete;
	nova_PipeLine operator=(const nova_PipeLine &) = delete;

	void bind(VkCommandBuffer commandBuffer);
	static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

  private:
	static std::vector<char> readFile(const std::string &filepath);

	void createGraphicsPipeline(const PipelineConfigInfo &configInfo);

	void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

	nova_Device &device;
	VkPipeline graphicsPipeLine;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
};
} // namespace nova

#endif