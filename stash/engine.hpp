#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "object.hpp"
#include "descriptors.hpp"
#include "mesh_system.hpp"

#include "types.hpp"

#include <memory>
#include <functional>
#include <vector>

namespace Nova {

class Engine {
public:
  Engine();
  ~Engine();

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  Device& getDevice() { return device; }
  
  void run();

  template<typename T>
  std::shared_ptr<T> getFirstObject() {
    for (const auto& obj : Objects) {
      if (auto castedObj = std::dynamic_pointer_cast<T>(obj)) {
        return castedObj;
      }
    }
    return nullptr;
  }

  ObjectList Objects;
  Resources* resources;
private:

  Window window;
  Device device;

  Settings settings;
};

}  // namespace Nova

#endif