#include "app.hpp"

namespace dvc {
void App::run() {
    while(!window.shouldClose()) {
        glfwPollEvents();
    }
}
}