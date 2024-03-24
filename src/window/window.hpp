#ifndef WINDOW_HPP
#define WINDOW_PP

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <string>

namespace dvc { //dynamic voxel craft
class dvc_window {
public:
    dvc_window(int w, int h, std::string name);
    
    bool shouldClose() {return glfwWindowShouldClose(window); }
    
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    dvc_window(const dvc_window & ) = delete;
    dvc_window &operator=(const dvc_window & ) = delete;

    ~dvc_window();
 private:

    void initWindow();

    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;
};
     
} // namespace dvc (dynamic voxel craft)


#endif