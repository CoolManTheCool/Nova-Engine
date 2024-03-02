#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "device.hpp"

//I have std
#include <string>
#include <vector>

namespace dvc {

    struct PipeLineConfigInfo {
        
    };
    
class dvc_PipeLine {
public:
    dvc_PipeLine(
    dvc_Device &device,
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipeLineConfigInfo configInfo);

    dvc_PipeLine(const dvc_PipeLine&) = delete;
    void operator=(const dvc_PipeLine&) = delete;

    static PipeLineConfigInfo defaultPipeLineConfigInfo(uint32_t width, uint32_t height);

private:
    static std::vector<char> readFile(const std::string& filepath);

    void createGraphicsPipeLine(
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipeLineConfigInfo configInfo);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    dvc_Device& device;
    VkPipeline graphicsPipeLine;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
} // namespace dvc


#endif