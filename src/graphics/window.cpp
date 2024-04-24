#include "window.hpp"
#include <stdexcept>
#include "util.hpp"

namespace nova {
nova_window::nova_window() {
    initWindow();
}

void nova_window::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(Settings.width, Settings.height, Settings.title.c_str(), nullptr, nullptr);
}

void nova_window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}


nova_window::~nova_window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

}