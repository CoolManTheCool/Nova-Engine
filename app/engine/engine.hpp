#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "device.hpp"
#include "object.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "descriptors.hpp"
#include "mesh_system.hpp"
#include "console.hpp"

#include "macros.hpp"

#include <memory>
#include <functional>
#include <vector>

namespace nova {

struct EngineConfig {
  Resources resources;
  Settings settings;
};

typedef std::vector<std::shared_ptr<nova_Object>> ObjectList;

struct LoopContext;

class NOVA_ENGINE_API Engine {
public:

  Engine(EngineConfig* config);
  ~Engine();

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  nova_Device& getDevice() { return device; }
  
  void run(bool (*loop)(LoopContext));

  template<typename T>
  std::shared_ptr<T> getFirstObject() {
    for (const auto& obj : Objects) {
      if (auto castedObj = std::dynamic_pointer_cast<T>(obj)) {
        return castedObj;
      }
    }
    return nullptr;
  }

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

struct LoopContext {
    float frameTime;
    ObjectList& objects;
    Engine& engine;

    LoopContext(float ft, ObjectList& objs, Engine& eng)
        : frameTime(ft), objects(objs), engine(eng) {}
};

}  // namespace nova

#endif