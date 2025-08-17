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

class MeshSystem;
class Camera;

class Graphics {
friend class Object;
friend class Camera;
public:
    Graphics(Settings& settings, Resources& resources);
    ~Graphics();

    void init();

    // Prevent copying
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    void renderFrame(ObjectList& objects, float frameTime);
    bool shouldClose() const;
private:
    Settings&   settings;
	Resources&  resources;
    Renderer*   renderer;
    MeshSystem* meshSystem;

    std::unique_ptr<DescriptorPool> globalPool{};

  	std::vector<std::unique_ptr<Buffer>> UBOBuffers;
  	std::unique_ptr<Buffer> globalUBOBuffer;
    std::vector<VkDescriptorSet> globalDescriptorSets;
};

} // namespace Nova

#endif // GRAPHICS_HPP