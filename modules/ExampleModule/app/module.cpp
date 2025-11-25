#include "module.hpp"
#include <iostream>

void ExampleModule::OnLoad() {
    std::cout << "ExampleModule loaded!\n";
}

void ExampleModule::OnUnload() {
    std::cout << "ExampleModule unloaded!\n";
}

// Factory function for runtime loading
extern "C" INovaModule* CreateModule() {
    return new ExampleModule();
}
