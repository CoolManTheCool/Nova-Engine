# Nova Engine

## NOTE: This is still in development.
There is extremely limited functionality and lacks any ability to develop a playable game.

## Overview

Nova Engine is the key component in a project called Ignis, Ingis is a game distrobution platform similar to roblox, while Nova Engine is the game engine behind Ingnis.

Nova Engine is a powerful and modern game development framework built with Vulkan. Designed to provide an intuitive and efficient way to create games, this engine allows you to build complex games with simple controls. Featuring an object oriented structure, automatic resource loading, and drag-and-drop simplicity, Nova Engine is perfect for both novice and experienced game developers.

## Features

- **Vulkan-Based Rendering**: Leverage the full power of Vulkan for high-performance graphics.
- **Object-Oriented Design**: Easily manage game objects and components with a clean, modular OOP structure.
- **Automatic Resource Loading**: Simply drop your assets into `src/resources/` and they are instantly available in your game.
- **Drag-and-Drop Simplicity**: Streamline your development process with easy-to-use drag-and-drop functionality for resources.
- **Futuristic Capabilities**: Cutting-edge features designed to support advanced game mechanics and graphics.
- **Project Structure Rethinked**: The Object-Oriented Design of this project allows for simple yet powerful parallelized systems.

## Getting Started

### Prerequisites

Before you begin, ensure you have met the following requirements:
- Operating System: Linux
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) installed
- [GLM](https://github.com/g-truc/glm) installed
- glslc
- C++ Compiler (Ideally GCC)
- CMake

For example on an Ubuntu based system:
```bash
sudo apt update
sudo apt install cmake build-essential libvulkan-dev vulkan-tools libglfw3 libglfw3-dev libglm-dev ninja-build
```

### Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/CoolManTheCool/Nova-Engine.git
   cd NovaEngine
   ```

2. **Make sure to initalize git submodules**
   ```bash
   git submodule update --init --recursive
   ```

3. **Build the Project**
   ```bash
   ./resources.sh
   mkdir -p build && cd build
   cmake -G Ninja ../app
   cmake --build .
   ```

### Using Nova Engine in Your Project

To use Nova Engine in your project, follow these steps:

1. **Include the Engine Header**
   ```cpp
   #include "nova/engine.hpp"
   ```

2. **Write your project**
 
   TODO: Finish this, and link it to the wiki.

4. **Run Your Game**
   Compile and run your game as usual. Nova Engine will handle the rest.

### Directory Structure

- **src/engine/**: Source code for the engine.
- **src/resources/**: Place your game assets (textures, models, sounds, etc.) here for automatic loading.
- **src/game/**: Where your game's code is defined.
- **build/**: Directory where the build output will be generated.

### Example

Here's a simple example of how to create a window and run the engine:
-# This example is a placeholder and does not reflect the actual project, please look at the examples.

```cpp
#include "nova/engine.hpp"

void render() {
 for(autoobject : Objects) {
  draw(object);
 }
}

struct data {
 std::vector<object_t> Objects;
}

int main() {
    NovaEngine::Engine engine;
    Settings.width = 1920;
    Settings.height = 1080;
    Settings.title = "Nova Engine Demo";
    engine.initialize();
    engine.setRenderCallback(render);
    
    Objects.push_back(object_t::new_object());
    engine.run();
    return 0;
}
```

### Contributing

Contributions are welcome! Please follow these steps to contribute:
1. Fork the repository.
2. Create a new branch.
3. Make your changes.
4. Commit your changes.
5. Push to the branch.
6. Create a pull request.

### License

This project is licensed under Nova Studios General Software License. See the [LICENSE](LICENSE) file for details.
Usage of this software in any way subjects you to this license. 

### Acknowledgements

- [Vulkan](https://www.vulkan.org/)
- [GLFW](https://www.glfw.org/)
- [GLM](https://github.com/g-truc/glm)
- [OpenAL](https://www.openal.org/) (Temporarily unused)

## Contact

For any inquiries, please contact [NoahGrimes0915@gmail.com](mailto:NoahGrimes0915@gmail.com).

---

Thank you for using Nova Engine! We look forward to seeing the amazing games you create.