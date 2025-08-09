#pragma once

#include "object.hpp"
#include "mesh.hpp"
#include "graphics.hpp"

namespace Nova {

struct PushMeshData {
  	glm::mat4 modelMatrix{1.f};
  	glm::mat4 normalMatrix{1.f};
	float roughness{0.5f};
};

class MeshObject : public Object {
public:
	float roughness{0.5f};
    void setModel(std::shared_ptr<Mesh> model);
	std::shared_ptr<Nova::Mesh> model{};

    unsigned int getObjectType() override;
  	void render(RenderData& renderData) override;
};

} // namespace Nova