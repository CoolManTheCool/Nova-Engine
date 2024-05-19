#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "model.hpp"
#include <memory>

namespace nova {

struct TransformComponent {
  glm::vec3 translation{};  // (position offset)
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  glm::mat4 mat4();
  glm::mat3 normalMatrix();
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
	void setModel(nova_Device *device, const nova_Model::Builder &builder) {
    model = std::make_shared<nova_Model>(*device, builder);
}

	std::shared_ptr<nova::nova_Model> model{};
	glm::vec3 color{};
	TransformComponent transform{};

  private:
	nova_Object(id_t id) : id(id) {}
	id_t id;
};

} // namespace nova

#endif