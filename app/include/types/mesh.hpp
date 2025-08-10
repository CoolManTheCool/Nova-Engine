#pragma once

// Public Facing

#include "types.hpp"

#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace Nova {

class Mesh {
friend class MeshObject;
friend class Resources;
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

	Mesh(const Mesh& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;

	~Mesh();

	// Pass through into meshBackend
	void bind(RenderData& renderData);
	void draw(RenderData& renderData);

private:
    Mesh(std::vector<Mesh::Vertex>& vertices,
        std::vector<uint32_t>& indices, Device& device);

    std::vector<Mesh::Vertex>& vertices;
    std::vector<uint32_t>& indices;
    
    std::unique_ptr<Buffer> vertexBuffer;
    std::unique_ptr<Buffer> indexBuffer;

    uint32_t vertexCount = 0;
    uint32_t indexCount  = 0;
    bool hasIndexBuffer  = false;

	Device& device;
    
	void reconstruct();

    Mesh(const Mesh& other);
    Mesh& operator=(const Mesh& other);
};

} // namespace Nova