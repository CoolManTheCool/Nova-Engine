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

class GUI_System_T {
public:
    void Init_ImGui(Device& device, Window& window, Renderer& renderer, VkDescriptorPool& imguiPool);
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
        return std::any_cast<T>(&it->second);
    }
    template<typename T>
    T getBindingValue(const std::string& name) {
        auto it = getBindingPointer<T>(name);
        return std::any_cast<T>(*it);
    }

    std::any* setBinding(const std::string& name, std::any value = false);
    
    bool checkBinding(const std::string& name);
    void registerWindow(std::function<void()> func);

private:
    Device* device = nullptr;
    Window* window = nullptr;
    Renderer* renderer = nullptr;
    VkDescriptorPool* imguiPool = nullptr;

    std::map<std::string, std::any> bindings;
    std::vector<std::function<void()>> windows; 
};

#ifndef GUI_SYSTEM_EXTERN
#define GUI_SYSTEM_EXTERN
extern GUI_System_T GUI;
#endif

} // namespace Nova

#endif // GUI_SYSTEM_HPP
