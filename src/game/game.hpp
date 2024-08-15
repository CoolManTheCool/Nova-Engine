#ifndef GAME_HPP
#define GAME_HPP

#include "device.hpp"
#include "object.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "descriptors.hpp"
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

// std
#include <memory>
#include <vector>

namespace nova {
class Game {
 public:

  Game();
  ~Game();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  void run();

 private:
  void loadGameObjects();
  void init_imgui();

  nova_Window window{};
  nova_Device device{window};
  nova_Renderer Renderer{window, device};

  VkDescriptorPool imguiPool;
  std::unique_ptr<nova_DescriptorPool> globalPool{};
  std::vector<std::shared_ptr<nova_Object>> Objects;
};
}  // namespace nova

#endif