#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <string>

namespace nova {
class nova_window {
public:
    nova_window();
    
    bool shouldClose() {return glfwWindowShouldClose(window); }
    
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    nova_window(const nova_window & ) = delete;
    nova_window &operator=(const nova_window & ) = delete;

    ~nova_window();
 private:

    void initWindow();
    Settings settings;
    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;
};
     
} // namespace nova (dynamic voxel craft)


#endif