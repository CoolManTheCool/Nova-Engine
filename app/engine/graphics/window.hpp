#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "utility/util.hpp"
#include <string>
#include <iostream>

//we want to immediately abort when there is an error. In normal engines this would give an error message to the user, or perform a dump of state.

#define VK_CHECK(x)                                                    \
	do                                                                 \
	{                                                                  \
		VkResult err = x;                                              \
		if (err)                                                       \
		{                                                              \
			std::cout <<"Detected Vulkan error: " << err << std::endl; \
			abort();                                                   \
		}                                                              \
	} while (0)

namespace Nova {
class Window {
  public:
	Window(Settings settings);

	bool shouldClose() { return glfwWindowShouldClose(window); }
	VkExtent2D getExtent() {
		return {static_cast<uint32_t>(settings->width),
				static_cast<uint32_t>(settings->height)};
	};
	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	bool wasWindowResized() const { 
		return frameBufferResized; 
	}
	void resetWindowResizedFlag() { 
		frameBufferResized = false; 
	}
	GLFWwindow *getWindow() const { return window; }

	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;

	~Window();

  private:
	void initWindow();
	static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);

	bool frameBufferResized = false;

	Settings* settings;

	GLFWwindow *window;
};

} // namespace Nova

#endif