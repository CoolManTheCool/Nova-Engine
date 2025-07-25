#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "macros.hpp"

#include "object.hpp"

namespace Nova {

class NOVA_ENGINE_API Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    // Prevent copying
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void renderFrame(ObjectList& objects);

private:

}

} // namespace Nova


#endif // RENDERER_HPP