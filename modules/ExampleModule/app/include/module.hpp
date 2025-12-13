#pragma once
#include "core/nova_module.hpp"

class ExampleModule : public INovaModule {
public:
    std::string GetName() const override { return "ExampleModule"; }

    void OnLoad() override;
    void OnUnload() override;
};
