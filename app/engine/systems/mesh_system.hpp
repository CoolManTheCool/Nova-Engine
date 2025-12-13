#pragma once

// Private facing

#include "device.hpp"
#include "pipeline.hpp"
#include "objects/camera.hpp"
#include "frame_info.hpp"
#include "utility/resources.hpp"

#include <memory>
#include <vector>

namespace Nova {

class MeshSystem : public Object {
public:
  	MeshSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, Resources& resources);
  	~MeshSystem();

  	MeshSystem(const MeshSystem &) = delete;
  	MeshSystem &operator=(const MeshSystem &) = delete;

  	void render(FrameInfo &frameInfo);
  	using Object::render; // To avoid hiding the base class render method

  	/**
	 * @brief Gets the static identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
	static const char* getStaticObjectType() { return "Mesh System"; }

	/**
	 * @brief Gets the identifier for the type of object
	 * @return const char*, 0 if generic.
	 */
  	const char* getObjectType() const override { return getStaticObjectType(); }

private:
  	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  	void createPipeline(VkRenderPass renderPass, Resources& resources);

  	Device& device;

  	std::unique_ptr<Pipeline> pipeline;
  	VkPipelineLayout pipelineLayout;
};
}  // namespace ~~lve~~nova