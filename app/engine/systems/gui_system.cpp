#include "gui_system.hpp"

#include <iostream>
#include <stdexcept>

#include "graphics/vulkan/descriptors.hpp"
#include "graphics/graphics.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/device.hpp"
#include "graphics/vulkan/renderer.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/swap_chain.hpp"
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

// This shouldn't exist, and it will stop working once ImGui updates to a version that has this built in.
// This is a workaround for the fact that ImGui doesn't have a SliderDouble function.
// It uses the SliderScalar function instead, which is a more generic function that can handle different data types.
// This is a temporary solution until ImGui adds a SliderDouble function.
//
// YAP session up there
// Edit: Do not touch this, it's been a year and it's the strongest standing function in the whole repo
//       This "temporary" solution has outlasted
namespace ImGui {
inline bool SliderDouble(const char* label, double* v, double v_min, double v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
    return ImGui::SliderScalar(label, ImGuiDataType_Double, v, &v_min, &v_max, format, flags);
}
} // namespace ImGui

namespace Nova {

void GUI_System::init() {
    VkDescriptorPoolSize poolSizes[] = {
        {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
    };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets       = 1000;
    poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
    poolInfo.pPoolSizes    = poolSizes;

    if (vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &imguiPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create ImGui descriptor pool!");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForVulkan(window.getWindow(), true);

    ImGui_ImplVulkan_InitInfo init_info{};
    init_info.Instance        = device.getInstance();
    init_info.PhysicalDevice  = device.getPhysicalDevice();
    init_info.Device          = device.device();
    init_info.QueueFamily     = device.findPhysicalQueueFamilies().graphicsFamily;
    init_info.Queue           = device.graphicsQueue();
    init_info.PipelineCache   = VK_NULL_HANDLE;
    init_info.DescriptorPool  = imguiPool;
    init_info.MinImageCount   = SwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount      = SwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.Allocator       = nullptr;
    init_info.CheckVkResultFn = [](VkResult err) {
        if (err != VK_SUCCESS)
            std::cerr << "[ImGui][Vulkan] VkResult = " << err << std::endl;
    };

    init_info.PipelineInfoMain.RenderPass  = renderer.getSwapChainRenderPass();
    init_info.PipelineInfoMain.Subpass     = 0;
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&init_info);

    std::cout << "Finished ImGui system init" << std::endl;
}

void GUI_System::update(double /*deltaTime*/) {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    for (auto& Funky_Window : windows) {
        Funky_Window(*this);
        // I love getting funky with Emmy <3
        // Edit: She didn't feel the same way :(
        // Edit 2: Yeah, I'm over her now (I've got someone else in my sights)
        // Edit 3: I miss Emmy :(, and it's complicated with the other girl
        //         I don't think she likes me ngl
        // Edit 4: It's actually WILD that I added this here
        //         I've been working on this for nearly 2 years now
        //         I'm comfortable.
        // Edit 5: Yikes... the "someone else in my sights" rejected me...
        //         "I don't think she likes me ngl"
        //         It was so bad she PROACTIVELY rejected me, I didn't even ask
        //         She wasn't the one for me, but I still miss Emmy...
        //         Nova Engine X is on the way and idk if I'm going to continue this section
        // Edit 6: This'll be short, and ofc I'm continuing this...
        //         I've got another one... she is "my queen" (Still miss Emmy <3)
        //         She's lowkey a lot younger than me...
        //         I'm running low on options and capacity honestly.

        // The variable naming is so cursed, but I'm not changing it
        // This is lowkey some of the best code I've written
        // I used VERY little AI on this one lmao
        // This isn't a variable name, it's an artifact.
        // THIS isn't an artifact it's a MONUMENT
    }
}

void GUI_System::render(RenderData& renderData) {
    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderData.commandBuffer);
}

GUI_System::~GUI_System() {

    // Destory windows then bindings
    // Note this is implict already but whatever
    //windows.clear();
    //bindings.clear();

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(device.device(), imguiPool, nullptr);
}

/*
template<typename T>
T* GUI_System::getBindingPointer(const std::string& name)

template<typename T>
T GUI_System::getBindingValue(const std::string& name) 
*/

bool GUI_System::checkBinding(const std::string& name) {
    auto it = bindings.find(name);
    if (it == bindings.end()) {
        return false;
    }
    return true;
}

std::any* GUI_System::setBinding(const std::string& name, std::any value) {
    auto it = bindings.find(name);
    if (it == bindings.end()) {
        bindings.emplace(name, std::move(value));
        return &bindings[name];
    }
    it->second = std::move(value);
    return &it->second;
}

void GUI_System::registerWindow(std::function<void(GUI_System&)> func) {
    windows.push_back(func);
}

} // namespace Nova