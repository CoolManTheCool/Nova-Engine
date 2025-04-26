#ifndef MESH_OBJECT_HPP
#define MESH_OBJECT_HPP

#include "object.hpp"

namespace nova {

struct PushMeshData {
  glm::mat4 modelMatrix{1.f};
  glm::mat4 normalMatrix{1.f};
};

class MeshObject : public nova_Object {
public:
    void setModel(nova_Device &device, const nova_Model::Builder builder);
	std::shared_ptr<nova::nova_Model> model{};
    unsigned int getObjectType() override;
  	void update(float deltaTime) override;
  	void render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) override;
};

} // namespace nova

#endif // MESH_OBJECT_HPP