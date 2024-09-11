#include "window.hpp"
#include <stdexcept>

namespace nova {
nova_Window::nova_Window() { initWindow(); }

void nova_Window::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(Settings.width, Settings.height, Settings.title.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
}

void nova_Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface!");
	}
}

nova_Window::~nova_Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void nova_Window::frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
	auto novaWindow = reinterpret_cast<nova_Window *>(glfwGetWindowUserPointer(window));
	novaWindow->frameBufferResized = true;
	Settings.width = width;
	Settings.height = height;
}
} // namespace nova