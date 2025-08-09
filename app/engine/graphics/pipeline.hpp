#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "device.hpp"
#include "util.hpp"

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
	Pipeline(Device& device, const std::string &vert, const std::string &frag, const PipelineConfigInfo &configInfo);
	~Pipeline();
    Pipeline(const Pipeline &) = delete;
    Pipeline operator=(const Pipeline &) = delete;

	void bind(VkCommandBuffer commandBuffer);
	static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

  private:
	static std::vector<char> readFile(const std::string &filepath);

	void createGraphicsPipeline(const std::string &vert, const std::string &frag, const PipelineConfigInfo &configInfo);

	void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

	Device& device;
	VkPipeline graphicsPipeLine;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
};
} // namespace Nova

#endif