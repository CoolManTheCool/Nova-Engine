#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "device.hpp"
#include "object.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "descriptors.hpp"
#include "mesh_system.hpp"
#include "console.hpp"

#include <memory>
#include <vector>

namespace nova {

struct EngineConfig {
  Resources resources;
  Settings settings;
};

typedef std::vector<std::shared_ptr<nova_Object>> ObjectList;

class Engine {
public:

  Engine(EngineConfig* config);
  ~Engine();

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  nova_Device& getDevice() { return device; }
  
  void run();

  std::vector<std::shared_ptr<nova_Object>> Objects;
  Resources* resources;
private:

  nova_Window window;
  nova_Device device;
  nova_Renderer Renderer;

  VkDescriptorPool imguiPool = VK_NULL_HANDLE;  // Initialize to null
  std::unique_ptr<nova_DescriptorPool> globalPool{};

  Settings settings;

  std::vector<std::unique_ptr<nova_Buffer>> UBOBuffers;
  std::unique_ptr<nova_Buffer> globalUBOBuffer;
};
}  // namespace nova

#endif