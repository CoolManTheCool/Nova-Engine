#pragma once
#include <string>

class INovaModule {
public:
    virtual ~INovaModule() = default;

    // The module name (string, not const char*)
    virtual std::string GetName() const = 0;

    // Called when the module is loaded at runtime (optional for library-only modules)
    virtual void OnLoad() {}
    virtual void OnUnload() {}
};
