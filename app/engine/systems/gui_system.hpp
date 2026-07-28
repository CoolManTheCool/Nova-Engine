#pragma once

#include <any>
#include <map>
#include <vector>
#include <string>
#include <functional>

#include "graphics/graphics.hpp"
#include "graphics/vulkan/device.hpp"
#include "graphics/graphics.hpp"
#include "graphics/window.hpp"
#include "systems/system.hpp"

struct VkDescriptorPool_T;
typedef VkDescriptorPool_T* VkDescriptorPool;

namespace Nova {

class GUI_System : public System {
public:
    using System::System; // inherit System's constructors
    GUI_System(const GUI_System&)            = delete;
    GUI_System& operator=(const GUI_System&) = delete;

    ~GUI_System();

    void init() override;
    void update(double dT) override;
    void render(RenderData& renderData) override;

    /**
     * @brief Get a pointer to a binding
     * @tparam T The type of the binding
     * @param name The name of the binding
     * @return A pointer to the binding
     * @throws std::runtime_error if the binding does not exist or if is called with empty name
     */
    template <typename T>
    T* getBindingPointer(const std::string& name) {
        if (name.empty()) {
            throw std::runtime_error("getBindingPointer called with empty name");
        }
        auto it = bindings.find(name);
        if (it == bindings.end()) {
            throw std::runtime_error("Binding not found: " + name);
        }
        return std::any_cast<T>(&it->second);
    }

    template <typename T>
    T getBindingValue(const std::string& name) {
        auto it = getBindingPointer<T>(name);
        return *it;
        //return std::any_cast<T>(*it);
    }

    std::any* setBinding(const std::string& name, std::any value = false);

    bool checkBinding(const std::string& name);
    void registerWindow(std::function<void(GUI_System&)> func);

private:
    VkDescriptorPool                              imguiPool;
    std::vector<std::function<void(GUI_System&)>> windows;
    std::map<std::string, std::any>               bindings;
};

} // namespace Nova