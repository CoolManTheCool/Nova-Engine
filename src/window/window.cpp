#include "window.hpp"

#include <stdexcept>

namespace dvc {
dvc_window::dvc_window(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
    initWindow();
}

void dvc_window::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

void dvc_window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}


dvc_window::~dvc_window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

}