#ifndef GUI_SYSTEM_HPP
#define GUI_SYSTEM_HPP

#include "types.hpp"

#include <any>
#include <map>
#include <vector>
#include <string>
#include <functional>

#include "graphics.hpp"
#include "device.hpp"

struct VkDescriptorPool_T;
typedef VkDescriptorPool_T *VkDescriptorPool;

namespace Nova {

class GUI_System : public Object {
public:
    GUI_System(Graphics& graphics);
    ~GUI_System();

    GUI_System(const GUI_System &) = delete;
    GUI_System &operator=(const GUI_System &) = delete;

    void update(float deltaTime) override;
    void render(RenderData& renderData) override;

    unsigned int getObjectType() override;

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
    Renderer& renderer;
    Device& device;
    VkDescriptorPool& imguiPool;

    std::map<std::string, std::any> bindings;
    std::vector<std::function<void()>> windows; 
};

} // namespace Nova

#endif // GUI_SYSTEM_HPP
