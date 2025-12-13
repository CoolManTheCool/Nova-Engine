#pragma once

#include "objects/object.hpp"
#include "components/mesh.hpp"
#include "core/graphics.hpp"

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
    
	/**
	 * @brief Gets the static identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
	static const char* getStaticObjectType() { return "Mesh Object"; }

	/**
	 * @brief Gets the identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
  	const char* getObjectType() const override { return getStaticObjectType(); }

  	void render(RenderData& renderData) override;
private:
	std::shared_ptr<Mesh> mesh{};
};

} // namespace Nova