#pragma once

// Private Facing

// Public Dependencies
#include "mesh.hpp"

// Private Dependencies
#include "device.hpp"

#include <vector>

namespace Nova {

struct MeshBuilder {
    std::vector<Mesh::Vertex> vertices{};
    std::vector<uint32_t> indices{};

	void loadModel(const std::string& filepath);
	Device& device;
};

}