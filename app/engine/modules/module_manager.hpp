#include <vector>
#include <memory>

// Public
#include "core/nova_module.hpp"
#include "objects/object.hpp"
#include "utility/resources.hpp"

namespace Nova {

class ModuleManager : public Object {
public:
    ModuleManager(Resources& resources);

    void UnloadModules();

    ~ModuleManager();

private:
    void LoadModules(const std::vector<std::string>& modulePaths);

    std::vector<std::unique_ptr<INovaModule>> modules;
};

} // namespace Nova