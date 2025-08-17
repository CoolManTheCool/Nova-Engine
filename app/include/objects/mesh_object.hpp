#pragma once

#include "object.hpp"
#include "components/mesh.hpp"
#include "graphics.hpp"

namespace Nova {

struct PushMeshData {
  	glm::mat4 modelMatrix{1.f};
  	glm::mat4 normalMatrix{1.f};
	float roughness{0.5f};
};

class MeshObject : public Object {
public:
	// Todo: Make private and apart of mesh
	float roughness{0.5f};

	/**
	 * @brief Transfers ownership of the mesh to this object
	 */
    void setMesh(std::shared_ptr<Mesh> mesh);
	
	/**
	 * @brief Get a unique copy of the mesh from this object.
	 */
	std::shared_ptr<Mesh> getMesh();
    
	unsigned int getObjectType() override;
  	void render(RenderData& renderData) override;
private:
	std::shared_ptr<Mesh> mesh{};
};

} // namespace Nova