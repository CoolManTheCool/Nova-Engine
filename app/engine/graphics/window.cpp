#include "window.hpp"
#include <stdexcept>

namespace Nova {
Window::Window(Settings settings) {
	this->settings = &settings;
	initWindow();
}

void Window::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(settings->width, settings->height, settings->title.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface!");
	}
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
	auto novaWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	novaWindow->frameBufferResized = true;
	novaWindow->settings->width = width;
	novaWindow->settings->height = height;
}
} // namespace Nova