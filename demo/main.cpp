#include "engine.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include "imgui/imgui.h"
// #include "imgui.h" if not building against engine source

struct AppConfig {
    float someValue = 42.0;
};

void gameLogic(const Nova::FrameCtx&, void* userData) {
    AppConfig& cfg = *reinterpret_cast<AppConfig*>(userData);

    ImGui::Begin("Demo Window");
    ImGui::Text("Hello, Nova Engine with ImGui!");
    ImGui::SliderFloat("Some Value", &cfg.someValue, 0.0f, 100.0f);
    ImGui::End();
}

int main(void) {
    Nova::EngineConfig config;
    config.appMajorVer = 1;
    config.appMinorVer = 1;
    config.appPatchVer = 0;
    config.title       = "Nova Engine Demo Application";
    AppConfig cfg;
    config.userData = &cfg;

    Nova::Engine engine(config);

    engine.loop(gameLogic);

    return 0;
}
