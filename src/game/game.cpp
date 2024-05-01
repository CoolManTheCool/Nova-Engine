#include "game.hpp"

#include "render_system.cpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

namespace nova {

Game::Game() { loadGameObjects(); }

Game::~Game() {}

void Game::run() {
  RenderSystem renderSystem{device, Renderer.getSwapChainRenderPass()};

  while (!window.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = Renderer.beginFrame()) {
      Renderer.beginSwapChainRenderPass(commandBuffer);
      renderSystem.renderGameObjects(commandBuffer, Objects);
      Renderer.endSwapChainRenderPass(commandBuffer);
      Renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(device.device());
}

void Game::loadGameObjects() {
  std::vector<nova_Model::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  for (int i = 0; i < 50; i++) {
	
  	auto triangle = nova_Object::createGameObject();
	triangle.setModel(&device, vertices);
  	triangle.color = {(float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX};
  	triangle.transform2d.scale = {(float)rand()/RAND_MAX/2, (float)rand()/RAND_MAX/2};
 	triangle.transform2d.rotation = .25f * M_PI * 2 + (float)rand()/RAND_MAX*2-1;
	triangle.transform2d.translation = {(float)rand()/RAND_MAX*2-1, (float)rand()/RAND_MAX*2-1};
	Objects.push_back(std::move(triangle));
  }
}

}  // namespace nova