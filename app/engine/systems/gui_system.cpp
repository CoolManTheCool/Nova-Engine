#include "gui_system.hpp"
#include "resources.hpp"

#include "descriptors.hpp"
#include "graphics.hpp"
#include "renderer.hpp"
#include "swap_chain.hpp"
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

// This shouldn't exist, and it will stop working once ImGui updates to a version that has this built in.
// This is a workaround for the fact that ImGui doesn't have a SliderDouble function.
// It uses the SliderScalar function instead, which is a more generic function that can handle different data types.
// This is a temporary solution until ImGui adds a SliderDouble function.
//
// YAP session up there
namespace ImGui {
    inline bool SliderDouble(const char* label, double* v, double v_min, double v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        return ImGui::SliderScalar(label, ImGuiDataType_Double, v, &v_min, &v_max, format, flags);
    }
} // namespace ImGui

namespace Nova {

GUI_System::GUI_System(Graphics& graphics)
    : renderer(graphics.getRenderer()), device(renderer.getDevice()), imguiPool(graphics.imguiPool) {

    Window& window = renderer.getWindow();

    // Create descriptor pool for ImGui
    VkDescriptorPoolSize poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
    poolInfo.pPoolSizes = poolSizes;

    if (vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &imguiPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create ImGui descriptor pool!");
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Platform/Renderer bindings
    ImGui_ImplGlfw_InitForVulkan(window.getWindow(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device.getInstance();
    init_info.PhysicalDevice = device.getPhysicalDevice();
    init_info.Device = device.device();
    init_info.QueueFamily = device.findPhysicalQueueFamilies().graphicsFamily;
    init_info.Queue = device.graphicsQueue();
    init_info.DescriptorPool = imguiPool;
    init_info.MinImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.RenderPass = renderer.getSwapChainRenderPass();

    ImGui_ImplVulkan_Init(&init_info);

    // Upload Fonts
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device.getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device.device(), &allocInfo, &command_buffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &beginInfo);

    ImGui_ImplVulkan_CreateFontsTexture();

    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command_buffer;

    vkQueueSubmit(device.graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.graphicsQueue());

    vkFreeCommandBuffers(device.device(), device.getCommandPool(), 1, &command_buffer);

    ImGui_ImplVulkan_DestroyFontsTexture();
}

void GUI_System::update(float) {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    std::cout << "Updating ImGui..." << std::endl;

    for (auto &Funky_Window : windows) {
        Funky_Window(); // I love getting funky with Emmy <3
                        // Edit: She didn't feel the same way :(
        std::cout << "Running ImGui window..." << std::endl;
    }
}

void GUI_System::render(RenderData& renderData) {
    std::cout << "Rendering ImGui..." << std::endl;
    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderData.commandBuffer);
}

unsigned int GUI_System::getObjectType() {
    return SYSTEM_TYPE_GUI;
}

GUI_System::~GUI_System() {

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

void GUI_System::registerWindow(std::function<void()> func) {
    windows.push_back(func);
}

} // namespace Nova
