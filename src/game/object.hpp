#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "model.hpp"
#include <memory>

namespace nova {

struct Transform2dComponent {
  glm::vec2 translation{};  // (position offset)
  glm::vec2 scale{1.f, 1.f};
  float rotation;

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix{{c, s}, {-s, c}};

    glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
    return rotMatrix * scaleMat;
  }
};

class nova_Object {
  public:
	using id_t = unsigned int;
	static nova_Object createGameObject() {
		static id_t currentId = 0;
		return nova_Object{currentId++};
	}

	nova_Object(const nova_Object &) = delete;
	nova_Object &operator=(const nova_Object &) = delete;
	nova_Object(nova_Object &&) = default;
	nova_Object &operator=(nova_Object &&) = default;

	id_t getId() { return id; };

	void setModel(std::shared_ptr<nova::nova_Model> *newModel) { model = *newModel; }
	void setModel(nova_Device *device, const std::vector<nova_Model::Vertex>& vertices) {
    model = std::make_shared<nova_Model>(*device, vertices);
}

	std::shared_ptr<nova::nova_Model> model{};
	glm::vec3 color{};
	Transform2dComponent transform2d{};

  private:
	nova_Object(id_t id) : id(id) {}
	id_t id;
};

} // namespace nova

#endif