#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "device.hpp"

//I have std
#include <string>
#include <vector>

namespace nova {

    struct PipeLineConfigInfo {
        
    };
    
class nova_PipeLine {
public:
    nova_PipeLine(
    nova_Device &device,
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipeLineConfigInfo configInfo);

    nova_PipeLine(const nova_PipeLine&) = delete;
    void operator=(const nova_PipeLine&) = delete;

    static PipeLineConfigInfo defaultPipeLineConfigInfo(uint32_t width, uint32_t height);

private:
    static std::vector<char> readFile(const std::string& filepath);

    void createGraphicsPipeLine(
    const std::string& vertFilepath,
    const std::string& fragFilepath,
    const PipeLineConfigInfo configInfo);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    nova_Device& device;
    VkPipeline graphicsPipeLine;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
} // namespace nova


#endif