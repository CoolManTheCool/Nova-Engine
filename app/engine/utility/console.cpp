#include "console.hpp"
#include "gui_system.hpp"

namespace nova {

Console_T Console = Console_T();

Console_T::Console_T() {
    size_t max_elements = Settings.console_lines;
    if (max_elements < MAX_CONSOLE_ELEMENTS) max_elements = MAX_CONSOLE_ELEMENTS;

    elements.reserve(max_elements);
	for (int i = 0; i<20; i++) {
		console_element Element;

		Element.level = i % console_element::MAX_LOG_LEVEL;
		Element.text = "Some example text";

		elements.push_back(Element);
	}

}

void Console_T::RegisterBindings() {

    GUI.setBinding("Console Open", true);
	GUI.setBinding("Console Key Down", false);
	GUI.setBinding("Auto Scroll", true);
	GUI.setBinding("Option 2 Checkbox", false);

}

void Console_T::RegisterWindow(nova_Window* window) {
	char** _filter = &filter;
	char* command = new char[100]();
	std::vector<console_element>* _elements = &elements;

    GUI.registerWindow([window, _filter, _elements, command]() {
		// GUI.getBindingValue<>("")
		if(glfwGetKey(window->getWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS && !GUI.getBindingValue<bool>("Console Key Down")) {
			*GUI.getBindingPointer<bool>("Console Open") = !GUI.getBindingValue<bool>("Console Open");
			*GUI.getBindingPointer<bool>("Console Key Down") = true;
		} else if(glfwGetKey(window->getWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE) {
			*GUI.getBindingPointer<bool>("Console Key Down") = false;
		}

		if (GUI.getBindingValue<bool>("Console Open") == true) {
			ImGui::Begin("Console Window", GUI.getBindingPointer<bool>("Console Open"));
			ImGui::Text("Console Window!!!");
			if(ImGui::Button("Options")) ImGui::OpenPopup("OptionsDropdown");
			ImGui::SameLine();
			if(ImGui::Button("Clear")) {}
			ImGui::SameLine();
			if(ImGui::Button("Copy")) {}
			//ImGui::InputTextWithHint("Filter", "Filter", text, 25);
			ImGui::InputTextWithHint("##Filter" /*Empty Label*/, "Filter", *_filter, 25);
			ImVec2 logContentSize = ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemSpacing.y); // Reserve space for the input bar
    		ImGui::BeginChild("LogContent", logContentSize, false, ImGuiWindowFlags_HorizontalScrollbar);
			int index = 0;
            for (const auto& element : *_elements) {
    			auto time = std::chrono::system_clock::to_time_t(element.sent);
    			std::tm* tm_time = std::localtime(&time);

    			// Convert timestamp to a string
    			char timeBuffer[100];
    			std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", tm_time);

    			// Determine log level and set the log label
    			const char* levelStr = "";
    			ImVec4 levelColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Default color (Gray)

    			switch (element.level) {
    			    case console_element::INFO:
    			        levelStr = "INFO";
    			        levelColor = ImVec4(0.2f, 0.8f, 0.2f, 1.0f); // Green for INFO
    			        break;
    			    case console_element::DEBUG:
    			        levelStr = "DEBUG";
    			        levelColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Dark Gray for DEBUG
    			        break;
    			    case console_element::WARNING:
    			        levelStr = "WARNING";
    			        levelColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow for WARNING
    			        break;
    			    case console_element::ERROR:
    			        levelStr = "ERROR";
    			        levelColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red for ERROR
    			        break;
    			    case console_element::CRITICAL:
    			        levelStr = "CRITICAL";
    			        levelColor = ImVec4(0.8f, 0.0f, 0.0f, 1.0f); // Dark Red for CRITICAL
    			        break;
    			}

    			ImGui::PushStyleColor(ImGuiCol_Text, levelColor); // Set Text Color

				if(element.details.size() > 0) {
					bool open = ImGui::CollapsingHeader((element.text + "##" + to_string(index)).c_str());
					if (open) {
				    	// Display log details
				    	ImGui::Text("[%s] [%s] %s", timeBuffer, levelStr, element.text.c_str());

				    	// Show additional details
				    	for (const auto& detail : element.details) {
				        	ImGui::Text("  %s", detail.c_str());
				    	}
					}
				} else {
					ImGui::Text("[%s] [%s] %s", timeBuffer, levelStr, element.text.c_str());
				}
		    	ImGui::PopStyleColor(); // Reset color
				index++;
			}

    		if (GUI.getBindingValue<bool>("Auto Scroll") && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        		ImGui::SetScrollHereY(1.0f);
    		}
			ImGui::EndChild();

			ImGui::InputTextWithHint("##Command" /*Empty Label*/, "Run \"/help\" to get started.", command, 50);

			if(ImGui::IsItemFocused() && glfwGetKey(window->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS) {
				
			}

			if(ImGui::BeginPopup("OptionsDropdown")) {
				bool hovered = ImGui::IsWindowHovered();

				ImGui::Checkbox("Auto Scroll", GUI.getBindingPointer<bool>("Auto Scroll"));
				ImGui::Checkbox("Test Checkbox 2", GUI.getBindingPointer<bool>("Option 2 Checkbox"));

				ImGui::EndPopup();
				if (!hovered) ImGui::CloseCurrentPopup();
			}

			ImGui::End();
		}	
	});
}
}