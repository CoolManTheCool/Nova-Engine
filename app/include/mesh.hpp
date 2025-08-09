#pragma once

// Public Facing

#include "types.hpp"

#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace Nova {

class Mesh { 
public:
	struct Vertex {
    	glm::vec3 position{};
    	glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};

		bool operator==(const Vertex& other) const {
			return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
		}
  	};

	Mesh(const Builder& builder);
	~Mesh() = default;

	//static std::unique_ptr<Mesh> createModelFromFile(Device& _device, const std::string &filepath);
	//static Builder createBuilderFromFile(const std::string &filepath) {Builder builder{}; builder.loadModel(filepath); return builder; }
	Mesh(const Mesh &) = delete;
	Mesh &operator=(const Mesh &) = delete;

	void bind(RenderData renderData);
	void draw(RenderData renderData);

private:
	void createVertexBuffers(const std::vector<Vertex> &vertices);
	void createIndexBuffers(const std::vector<uint32_t> &indices);

	MeshBackend* meshBackend;

	std::unique_ptr<Buffer> vertexBuffer;
	uint32_t vertexCount;

	bool hasIndexBuffer = false;
	std::unique_ptr<Buffer> indexBuffer;
	uint32_t indexCount;
};
} // namespace Nova