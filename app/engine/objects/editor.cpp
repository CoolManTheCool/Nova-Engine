#include "editor.hpp"
#include "gui_system.hpp"

namespace nova {

Editor_T::Editor_T() {

}

void Editor_T::RegisterBindings() {
    GUI.setBinding("Window Editor Open", false);
}

std::function<void()> Editor_T::RegisterWindow(nova_Window* window) {

    return [window]() {
	if (GUI.getBindingValue<bool>("Window Editor Open") == true) {
		ImGui::Begin("Editor", GUI.getBindingPointer<bool>("Window Editor Open"), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Editor Window");
    	ImGui::End();
	}};

    }


}