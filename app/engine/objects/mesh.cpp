// Implements
#include "mesh.hpp"
#include "mesh_object.hpp"

// Public Dependencies
#include "util.hpp"

// Private Dependencies
#include "renderer.hpp"
#include "buffer.hpp"
#include "mesh_builder.hpp"

#include <cassert>
#include <cstring>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace std {
template <>
struct hash<Nova::Mesh::Vertex> {
    size_t operator()(Nova::Mesh::Vertex const &vertex) const {
        size_t seed = 0;
        Nova::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    }
};

} // namespace std

namespace Nova {

// --- Mesh Implementation ---

Mesh::Mesh(std::vector<Mesh::Vertex>& vertices, std::vector<uint32_t>& indices, Device& device)
    : vertices(vertices), indices(indices), device(device) {
    
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");
    indexCount = static_cast<uint32_t>(indices.size());
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

Mesh::Mesh(const Mesh& other)
    : vertices(other.vertices), indices(other.indices), device(other.device) {

    vertexCount = static_cast<uint32_t>(vertices.size());
    indexCount = static_cast<uint32_t>(indices.size());
    hasIndexBuffer = indexCount > 0;

    reconstruct();
}

Mesh &Mesh::operator=(const Mesh &other)  {
    if (this == &other) return *this; // self-assignment guard

    vertices = other.vertices;
    indices = other.indices;
    
    // Device is already bound for some unholy reason

    return *this;
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

std::vector<VkVertexInputBindingDescription> getBindingDescriptions() {
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Mesh::Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescriptions;
}	

std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
	return {
		//location, binding, format, offset
		{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::Vertex, Mesh::Vertex::position)},
		{1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::Vertex, Mesh::Vertex::color)},
		{2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::Vertex, Mesh::Vertex::normal)},
		{3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Mesh::Vertex, Mesh::Vertex::uv)},
	};
}
        
void MeshBuilder::loadModel(const std::string &filepath) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	vertices.clear();
	indices.clear();

	std::unordered_map<Mesh::Vertex, uint32_t> uniqueVertices{};

	for(const auto &shape : shapes) {
		for (const auto &index : shape.mesh.indices) {
			Mesh::Vertex vertex{};

			if (index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				auto colorIndex = static_cast<std::vector<float>::size_type>(3 * index.vertex_index + 2);
				if (colorIndex < attrib.colors.size()) {
					vertex.color = {
					attrib.colors[colorIndex - 2],
					attrib.colors[colorIndex - 1],
					attrib.colors[colorIndex - 0],
					};
				} else {
					vertex.color = {1.f, 1.f, 1.f};
				}
			}
			if (index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}
			if (index.texcoord_index >= 0) {
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

// --- MeshObject Implementation ---

void MeshObject::setModel(std::shared_ptr<Mesh> mesh) {
    if(mesh) {
        this->mesh = std::make_shared<Mesh>(*mesh);
    } else {
        this->mesh.reset(); // reset to nullptr if null passed
    }
}

std::shared_ptr<Mesh> MeshObject::getModel() {
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
    model->bind(renderData);
    model->draw(renderData);
}

}  // namespace Nova