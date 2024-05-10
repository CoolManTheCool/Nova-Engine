#ifndef MODEL_HPP
#define MODEL_HPP

#include "device.hpp"
#include "buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace nova {
class nova_Model { 
public:
	struct Vertex {
    	glm::vec3 position{};
    	glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};

    	static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const Vertex& other) const {
			return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
		}
  	};

  	struct Builder {
    	std::vector<Vertex> vertices{};
    	std::vector<uint32_t> indices{};

		void loadModel(const std::string &filepath);
  	};

	nova_Model(nova_Device &device, const nova_Model::Builder &builder);
	~nova_Model() = default;

	static std::unique_ptr<nova_Model> createModelFromFile(nova_Device &_device, const std::string &filepath);

	nova_Model(const nova_Model &) = delete;
	nova_Model &operator=(const nova_Model &) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	void createVertexBuffers(const std::vector<Vertex> &vertices);
	void createIndexBuffers(const std::vector<uint32_t> &indices);

	nova_Device &device;

	std::unique_ptr<nova_Buffer> vertexBuffer;
	uint32_t vertexCount;

	bool hasIndexBuffer = false;
	std::unique_ptr<nova_Buffer> indexBuffer;
	uint32_t indexCount;
};
} // namespace nova

#endif