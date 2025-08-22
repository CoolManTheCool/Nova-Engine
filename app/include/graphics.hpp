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

struct VkDescriptorPool_T;
typedef VkDescriptorPool_T *VkDescriptorPool;

#ifndef VK_NULL_HANDLE
#define VK_NULL_HANDLE 0
#endif

namespace Nova {

class MeshSystem;
class GUI_System;

class Graphics {
friend class Object;
friend class GUI_System;
public:
    Graphics(Settings& settings, Resources& resources);
    ~Graphics();

    void init();

    // Prevent copying
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    Renderer& getRenderer();

    void renderFrame(ObjectList& objects, float frameTime);
    bool shouldClose() const;
private:
    Settings&   settings;
	Resources&  resources;
    Renderer*   renderer;
    MeshSystem* meshSystem;

    std::unique_ptr<DescriptorPool> globalPool{};
    VkDescriptorPool imguiPool = VK_NULL_HANDLE;

  	std::vector<std::unique_ptr<Buffer>> UBOBuffers;
  	std::unique_ptr<Buffer> globalUBOBuffer;
    std::vector<VkDescriptorSet> globalDescriptorSets;
    
};

} // namespace Nova

#endif // GRAPHICS_HPP