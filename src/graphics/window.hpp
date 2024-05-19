#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "util.hpp"
#include <string>

namespace nova {
class nova_Window {
  public:
	nova_Window();

	bool shouldClose() { return glfwWindowShouldClose(window); }
	VkExtent2D getExtent() {
		return {static_cast<uint32_t>(Settings.width),
				static_cast<uint32_t>(Settings.height)};
	};
	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	bool wasWindowResized() { return frameBufferResized; };
	void resetWindowResizedFlag() { frameBufferResized = false; }
	GLFWwindow *getWindow() const { return window; }

	nova_Window(const nova_Window &) = delete;
	nova_Window &operator=(const nova_Window &) = delete;

	~nova_Window();

  private:
	void initWindow();
	static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);

	bool frameBufferResized = false;

	GLFWwindow *window;
};

} // namespace nova

#endif