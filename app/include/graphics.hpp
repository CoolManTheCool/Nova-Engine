#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

// Forward facing

#include "types.hpp"
#include "object.hpp"
#include "util.hpp"
#include "resources.hpp"

#include <memory>

struct VkDescriptorSet_T;
typedef struct VkDescriptorSet_T* VkDescriptorSet;

namespace Nova {

class Graphics {
friend class Object;
public:
    Graphics(Settings& settings, Resources& resources);
    ~Graphics();

    void init();

    // Prevent copying
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    void renderFrame(ObjectList& objects, float frameTime);
private:
    Settings&  settings;
	Resources& resources;
    Renderer*  renderer;

    std::unique_ptr<DescriptorPool> globalPool{};

  	std::vector<std::unique_ptr<Buffer>> UBOBuffers;
  	std::unique_ptr<Buffer> globalUBOBuffer;
    std::vector<VkDescriptorSet> globalDescriptorSets;
};

} // namespace Nova

#endif // GRAPHICS_HPP