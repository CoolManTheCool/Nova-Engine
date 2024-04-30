#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "util.hpp"
#include <string>

namespace nova {
class nova_window {
  public:
	nova_window();

	bool shouldClose() { return glfwWindowShouldClose(window); }
	VkExtent2D getExtent() {
		return {static_cast<uint32_t>(Settings.width),
				static_cast<uint32_t>(Settings.height)};
	};
	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	bool wasWindowResized() { return frameBufferResized; };
	void resetWindowResizedFlag() { frameBufferResized = false; }

	nova_window(const nova_window &) = delete;
	nova_window &operator=(const nova_window &) = delete;

	~nova_window();

  private:
	void initWindow();
	static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);

	bool frameBufferResized = false;

	GLFWwindow *window;
};

} // namespace nova

#endif