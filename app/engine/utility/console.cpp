#include "console.hpp"
#include "gui_system.hpp"

namespace nova {

Console_T Console = Console_T();

Console_T::Console_T() {
    size_t max_elements = Settings.console_lines;
    if (max_elements < MAX_CONSOLE_ELEMENTS) max_elements = MAX_CONSOLE_ELEMENTS;

    elements.reserve(max_elements);

}

void Console_T::RegisterBindings() {

    GUI.setBinding("Console Open", true);
	GUI.setBinding("Option 1 Checkbox", false);
	GUI.setBinding("Option 2 Checkbox", true);

}

void Console_T::RegisterWindow() {
    GUI.registerWindow([this]() {
		// GUI.getBindingValue<>("")
        ImGui::Begin("Console Window", GUI.getBindingPointer<bool>("Console Open"));
		if (GUI.getBindingValue<bool>("Console Open") == true) {
			ImGui::Text("Console Window!!!");
			if(ImGui::Button("Options")) ImGui::OpenPopup("OptionsDropdown");
			ImGui::SameLine();
			if(ImGui::Button("Clear")) {}
			ImGui::SameLine();
			if(ImGui::Button("Copy")) {}
			ImGui::SameLine();
			ImGui::Text("Filter: ");
			ImGui::SameLine();
			//ImGui::InputTextWithHint("Filter", "Filter", text, 25);
			ImGui::InputTextWithHint("Filter", "Filter", filter, 25);
            

			if(ImGui::BeginPopup("OptionsDropdown")) {
				bool hovered = ImGui::IsWindowHovered();

				ImGui::Checkbox("Test Checkbox 1", GUI.getBindingPointer<bool>("Option 1 Checkbox"));
				ImGui::Checkbox("Test Checkbox 2", GUI.getBindingPointer<bool>("Option 2 Checkbox"));

				ImGui::EndPopup();
				if (!hovered) ImGui::CloseCurrentPopup();
			}

			ImGui::End();
		}	
	});
}
}