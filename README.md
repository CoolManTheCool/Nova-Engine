# Nova Engine

## NOTE: This is still in development.
The game structure is wrong.
There isn't even an engine header for you to link against.

## Overview

Welcome to Nova Engine, a powerful and modern game development framework built with Vulkan. Designed to provide an intuitive and efficient way to create games, this engine allows you to build complex game worlds as if you were snapping together LEGO bricks. Featuring an OOP structure, automatic resource loading, and drag-and-drop simplicity, Nova Engine is perfect for both novice and experienced game developers.

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
- C++ Compiler (GCC, Clang, MSVC)
- CMake (for building the project)

### Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/CoolManTheCool/Nova-Engine.git
   cd NovaEngine
   ```

2. **Install Dependencies**
   ```bash
   sudo apt install libglfw3-dev libglm-dev libopenal-dev vulkan-tools libvulkan-dev vulkan-validationlayers-dev glslc spirv-tools
   ```

3. **Build the Project**
   ```bash
   ./build.sh
   ```

### Using Nova Engine in Your Project

To use Nova Engine in your project, follow these steps:

1. **Include the Engine Header**
   ```cpp
   #include "nova-engine/engine.hpp"
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

```cpp
#include "nova-engine/engine.hpp"

void render() {
 for(auto &object : Objects) {
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

This project is licensed under Ignis Studio's Software License. See the [LICENSE](LICENSE) file for details.
Usage of this software in any way subjects you to this license. 

### Acknowledgements

- [Vulkan](https://www.vulkan.org/)
- [GLFW](https://www.glfw.org/)
- [GLM](https://github.com/g-truc/glm)
- [OpenAL](https://www.openal.org/)

## Contact

For any inquiries, please contact [NoahGrimes0915@gmail.com](mailto:NoahGrimes0915@gmail.com).

---

Thank you for using Nova Engine! We look forward to seeing the amazing games you create.
