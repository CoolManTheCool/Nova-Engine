#ifndef GAME_HPP
#define GAME_HPP

#include "device.hpp"
#include "object.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "descriptors.hpp"

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

  nova_window window{};
  nova_Device device{window};
  nova_Renderer Renderer{window, device};

  std::unique_ptr<nova_DescriptorPool> globalPool{};
  std::vector<nova_Object> Objects;
};
}  // namespace nova

#endif