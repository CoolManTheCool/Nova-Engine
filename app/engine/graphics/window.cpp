#include "graphics/window.hpp"

#include <iostream>

#include "vulkan/vulkan.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace Nova {

Window::Window(const EngineConfig& c) {
    glfwInit();

    state.width  = c.width;
    state.height = c.height;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(c.width, c.height, c.title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwSwapInterval(c.enableVSync);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
    std::cout << "Window created:" << "\n - Width: " << c.width << "\n - Height: " << c.height << "\n - Title: " << c.title << std::endl;
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

const WindowState& Window::getWindowState() const noexcept {
    return state;
}

void Window::pollEvents() const noexcept {
    glfwPollEvents();
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

VkExtent2D Window::getExtent() {
    return VkExtent2D{state.width, state.height};
}

bool Window::wasWindowResized() const {
    return frameBufferResized;
}

void Window::resetWindowResizedFlag() {
    frameBufferResized = false;
}

GLFWwindow* Window::getWindow() const {
    return window;
}

void Window::frameBufferResizeCallback(GLFWwindow* window_, int width, int height) {
    auto window                = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window_));
    window->frameBufferResized = true;
    window->state.width        = width;
    window->state.height       = height;
}

} // namespace Nova