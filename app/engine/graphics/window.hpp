#pragma once

#include "primatives/engine_config.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace Nova {

struct WindowState {
    uint32_t width;
    uint32_t height;
};

class Window {
public:
    explicit Window(const EngineConfig& config);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose();
    const WindowState& getWindowState() const noexcept;
    void pollEvents() const noexcept;

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    
    VkExtent2D getExtent();
    bool wasWindowResized() const;
    void resetWindowResizedFlag();
    GLFWwindow* getWindow() const;
private:
    static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);

	bool frameBufferResized = false;

    WindowState state;
    GLFWwindow* window = nullptr;
};

}
