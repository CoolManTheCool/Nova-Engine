#include "module_manager.hpp"

#include <iostream>
#include <filesystem>
#include <memory>
#include <vector>
#include <string>

#if defined(_WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace Nova {

ModuleManager::ModuleManager(Resources& resources) {
    LoadModules(resources.getModulePaths());
}

void ModuleManager::LoadModules(const std::vector<std::string>& modulePaths) {
    for (const auto& path : modulePaths) {
#if defined(_WIN32)
        HMODULE handle = LoadLibraryA(path.c_str());
        if (!handle) {
            std::cerr << "[ModuleManager] Failed to load module: " << path << "\n";
            continue;
        }

        auto createFunc = reinterpret_cast<INovaModule*(*)()>(
            GetProcAddress(handle, "CreateModule")
        );
        if (!createFunc) {
            std::cerr << "[ModuleManager] CreateModule function not found in: " << path << "\n";
            FreeLibrary(handle);
            continue;
        }

        std::unique_ptr<INovaModule> module(createFunc());
#else
        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "[ModuleManager] Failed to load module: " << path << "\n" << dlerror() << "\n";
            continue;
        }

        auto createFunc = reinterpret_cast<INovaModule*(*)()>(
            dlsym(handle, "CreateModule")
        );
        if (!createFunc) {
            std::cerr << "[ModuleManager] CreateModule function not found in: " << path << "\n";
            dlclose(handle);
            continue;
        }

        std::unique_ptr<INovaModule> module(createFunc());
#endif
        
        try {
            module->OnLoad();
        } catch (const std::exception& e) {
            std::cerr << "[ModuleManager] Exception during OnLoad of module " << module->GetName() << ": " << e.what() << "\n";
            continue;
        }

        std::cout << "[ModuleManager] Successfully loaded module: " << module->GetName() << "\n";

        // Store module
        modules.push_back(std::move(module));
        
    }
}

void ModuleManager::UnloadModules() {
    for (auto& module : modules) {
        module->OnUnload();
    }
    modules.clear();

}

ModuleManager::~ModuleManager() {
    UnloadModules();
}

} // namespace Nova