#ifndef GUI_SYSTEM_HPP
#define GUI_SYSTEM_HPP

#include "descriptors.hpp"
#include "renderer.hpp"
#include "swap_chain.hpp"
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

#include <any>
#include <map>
#include <vector>
#include <string>
#include <functional>

namespace nova {

class GUI_System_T {
public:
    void Init_ImGui(nova_Device* device, nova_Window* window, nova_Renderer* renderer, VkDescriptorPool* imguiPool);
    bool isRunning();
    void update();
    void render(VkCommandBuffer* commandBuffer);
    void Shutdown();

    template<typename T>
    T* getBindingPointer(const std::string& name) {
        auto it = bindings.find(name);
        if (it == bindings.end()) {
            throw std::runtime_error("Binding not found: " + name);
        }
        return any_cast<T>(&it->second);
    }
    template<typename T>
    T getBindingValue(const std::string& name) {
        auto it = getBindingPointer<T>(name);
        return any_cast<T>(*it);
    }

    std::any* setBinding(const std::string& name, std::any value = false);
    
    bool checkBinding(const std::string& name);
    void registerWindow(std::function<void()> func);

private:
    nova_Device* device = nullptr;
    nova_Window* window = nullptr;
    nova_Renderer* renderer = nullptr;
    VkDescriptorPool* imguiPool = nullptr;

    std::map<std::string, std::any> bindings;
    std::vector<std::function<void()>> windows; 
};

#ifndef GUI_SYSTEM_EXTERN
#define GUI_SYSTEM_EXTERN
extern GUI_System_T GUI;
#endif

} // namespace nova

#endif // GUI_SYSTEM_HPP