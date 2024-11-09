#include "gui_system.hpp"
#include "resources.hpp"

namespace nova {

GUI_System_T GUI;

void GUI_System_T::Init_ImGui(nova_Device* device, nova_Window* window, nova_Renderer* renderer, VkDescriptorPool* imguiPool) {
    if (isRunning()) throw std::runtime_error("ImGui has already been initialized!");

    this->device = device;
    this->window = window;
    this->renderer = renderer;
    this->imguiPool = imguiPool;

    // Vulkan descriptor pool setup
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
    poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = 100;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    if (vkCreateDescriptorPool(device->device(), &poolInfo, nullptr, imguiPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool for ImGui!");
    }

    // ImGui context initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Platform/Renderer bindings
    ImGui_ImplGlfw_InitForVulkan(window->getWindow(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device->getInstance();
    init_info.PhysicalDevice = device->getPhysicalDevice();
    init_info.Device = device->device();
    init_info.QueueFamily = device->findPhysicalQueueFamilies().graphicsFamily;
    init_info.Queue = device->graphicsQueue();
    init_info.DescriptorPool = *imguiPool;
    init_info.MinImageCount = nova_SwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount = nova_SwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.RenderPass = renderer->getSwapChainRenderPass();

    ImGui_ImplVulkan_Init(&init_info);

    // Upload Fonts
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device->device(), &allocInfo, &command_buffer);

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

    vkQueueSubmit(device->graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->graphicsQueue());

    vkFreeCommandBuffers(device->device(), device->getCommandPool(), 1, &command_buffer);

    ImGui_ImplVulkan_DestroyFontsTexture();
}

bool GUI_System_T::isRunning() {
    return (device != nullptr);
}

void GUI_System_T::update() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    for (auto &Funky_Window : windows) {
        Funky_Window();
    }
}

void GUI_System_T::render(VkCommandBuffer* commandBuffer) {
    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *commandBuffer);
}

void GUI_System_T::Shutdown() {
    if (!isRunning()) throw std::runtime_error("Tried shutting down ImGui while not running!");

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(device->device(), *imguiPool, nullptr);
}
/*

template<typename T>
T* GUI_System_T::getBindingPointer(const std::string& name)

template<typename T>
T GUI_System_T::getBindingValue(const std::string& name) 

*/
bool GUI_System_T::checkBinding(const std::string& name) {
    auto it = bindings.find(name);
    if (it == bindings.end()) {
        return false;
    }
    return true;
}

std::any* GUI_System_T::setBinding(const std::string& name, std::any value) {
    auto it = bindings.find(name);
    if (it == bindings.end()) {
        bindings.emplace(name, std::move(value));
        return &bindings[name];
    }
    it->second = std::move(value);
    return &it->second;
}

void GUI_System_T::registerWindow(std::function<void()> func) {
    windows.push_back(func);
}

} // namespace nova
