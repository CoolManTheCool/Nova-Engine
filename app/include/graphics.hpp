#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

// Forward facing

#include "types.hpp"
#include "object.hpp"
#include "util.hpp"

#include <memory>

namespace Nova {

class Graphics {
friend class Object;
public:
    Graphics(Settings& settings);
    ~Graphics();

    void init();

    // Prevent copying
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    void renderFrame(ObjectList& objects);
private:
    Settings& settings;
    Renderer* renderer;

    std::unique_ptr<DescriptorPool> globalPool{};

  std::vector<std::unique_ptr<Buffer>> UBOBuffers;
  std::unique_ptr<Buffer> globalUBOBuffer;
};

} // namespace Nova

#endif // GRAPHICS_HPP