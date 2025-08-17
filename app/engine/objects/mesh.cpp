// Implements
#include "components/mesh.hpp"
#include "mesh_object.hpp"

// Public Dependencies
#include "util.hpp"

// Private Dependencies
#include "renderer.hpp"
#include "buffer.hpp"

#include <cassert>
#include <cstring>
#include <unordered_map>
#include <memory>

namespace Nova {

// --- Mesh Implementation ---

Mesh::~Mesh() {
    // unique_ptrs will be automatically destroyed
}

Mesh::Mesh(const std::vector<Mesh::Vertex>& vertices,
           const std::vector<uint32_t>& indices,
           Device& device)
    : vertices(vertices), indices(indices), device(device) {

    vertexCount = static_cast<uint32_t>(this->vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");
    indexCount = static_cast<uint32_t>(this->indices.size());
    hasIndexBuffer = indexCount > 0;

    reconstruct();
}

void Mesh::reconstruct() {
    VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertexCount;
    uint32_t vertexSize = sizeof(vertices[0]);

    Buffer stagingVertexBuffer{
        device,
        vertexSize,
        vertexCount,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };
    stagingVertexBuffer.map();
    stagingVertexBuffer.writeToBuffer((void *)vertices.data());

    vertexBuffer = std::make_unique<Buffer>(
        device, vertexSize, vertexCount,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    device.copyBuffer(stagingVertexBuffer.getBuffer(), vertexBuffer->getBuffer(), vertexBufferSize);

    if (hasIndexBuffer) {
        VkDeviceSize indexBufferSize = sizeof(indices[0]) * indexCount;
        uint32_t indexSize = sizeof(indices[0]);

        Buffer stagingIndexBuffer{
            device, indexSize, indexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        stagingIndexBuffer.map();
        stagingIndexBuffer.writeToBuffer((void*) indices.data());

        indexBuffer = std::make_unique<Buffer>(
            device, indexSize, indexCount,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        device.copyBuffer(stagingIndexBuffer.getBuffer(), indexBuffer->getBuffer(), indexBufferSize);
    }
}

void Mesh::bind(RenderData& renderData) {
    VkBuffer buffers[] = { vertexBuffer->getBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(renderData.commandBuffer, 0, 1, buffers, offsets);
    if (hasIndexBuffer) {
        vkCmdBindIndexBuffer(renderData.commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
}

void Mesh::draw(RenderData& renderData) {
    if (hasIndexBuffer) {
        vkCmdDrawIndexed(renderData.commandBuffer, indexCount, 1, 0, 0, 0);
    } else {
        vkCmdDraw(renderData.commandBuffer, vertexCount, 1, 0, 0);
    }
}

std::shared_ptr<Mesh> Mesh::unique() {
    return std::shared_ptr<Mesh>(new Mesh(vertices, indices, device));
}

// --- MeshObject Implementation ---

void MeshObject::setMesh(std::shared_ptr<Mesh> mesh) {
    if(mesh) {
        this->mesh = mesh;
    } else {
        this->mesh.reset(); // reset to nullptr if null passed
    }
}

std::shared_ptr<Mesh> MeshObject::getMesh() {
    return mesh;
}

unsigned int MeshObject::getObjectType() {
    return OBJECT_TYPE_MESH;
}

void MeshObject::render(RenderData& renderData) {
    PushMeshData push{};
    push.modelMatrix  = transform.mat4();
    push.normalMatrix = transform.normalMatrix();
    push.roughness    = roughness;
    vkCmdPushConstants(
        renderData.commandBuffer,
        renderData.pipelineLayout,
        	VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(PushMeshData),
        &push);
    mesh->bind(renderData);
    mesh->draw(renderData);
}

};  // namespace Nova