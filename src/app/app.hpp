#ifndef APP_HPP
#define APP_HPP

#include "pipeline.hpp"

namespace dvc {
class App {
public:
    static constexpr int WIDTH  = 800;
    static constexpr int HEIGHT = 600;

    void run();

private:
    dvc_window window{WIDTH, HEIGHT, "Hello Vulkan"};
    dvc_Device device{window};
    dvc_PipeLine PipeLine{
    device,
    "/home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/vertex.vert.spv",
    "/home/noah/github/Dynamic-Voxel-Craft/src/resources/shaders/fragment.frag.spv",
    dvc_PipeLine::defaultPipeLineConfigInfo(WIDTH, HEIGHT)
    };
};

}

#endif