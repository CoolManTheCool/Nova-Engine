#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "macros.hpp"

#include "object.hpp"

#include <vector>
#include <memory>

namespace Nova {

// Forward declarations
struct Renderer;

class NOVA_ENGINE_API Engine {
public:
    Engine() = default;
    ~Engine() = default;

    // Prevent copying
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

private:
    ObjectList Objects;
    Renderer renderer;
};

} // namespace Nova


#endif // ENGINE_HPP