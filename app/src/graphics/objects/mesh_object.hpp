#include "object.hpp"

namespace nova {

struct PushMeshData {
  glm::mat4 modelMatrix{1.f};
  glm::mat4 normalMatrix{1.f};
};

class MeshObject : public nova_Object {
public:
    void setModel(nova_Device *device, const nova_Model::Builder builder);
    void render(VkCommandBuffer &commandBuffer) override;
	std::shared_ptr<nova::nova_Model> model{};
};

} // namespace nova