#include "mesh_object.hpp"

namespace nova {
void MeshObject::setModel(nova_Device *device, const nova_Model::Builder builder) {
    model = std::make_shared<nova_Model>(*device, builder);
}

unsigned int MeshObject::getObjectType() {
    return OBJECT_TYPE_MESH;
}

void MeshObject::render(VkPipelineLayout &pipelineLayout, VkCommandBuffer &commandBuffer) {
    PushMeshData push{};
    push.modelMatrix  = transform.mat4();
    push.normalMatrix = transform.normalMatrix();
    vkCmdPushConstants(
        commandBuffer,
        pipelineLayout,
        	VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(PushMeshData),
        &push);
    model->bind(commandBuffer);
    model->draw(commandBuffer);
}

void MeshObject::update(float deltaTime) {

}

}