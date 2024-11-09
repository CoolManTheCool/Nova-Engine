#include "editor.hpp"
#include "gui_system.hpp"
#include "object.hpp"

namespace nova {

#define min_dist_edit -10 // Minimum Distance the Editor allows.
#define max_dist_edit 10 // Maximum Distance the Editor allows.

Editor_T::Editor_T() {

}

void Editor_T::RegisterBindings() {
    GUI.setBinding("Window Editor Open", true);
    GUI.setBinding("Obj Num", 0);
}

std::function<void()> Editor_T::RegisterWindow(nova_Window* window) {

    std::vector<std::shared_ptr<nova_Object>> *objects = GUI.getBindingValue<std::vector<std::shared_ptr<nova_Object>>*>("Objects");
    assert(objects && "'Objects' is not bound or is null.");
    
    return [window, objects]() {
        int* objNum = GUI.getBindingPointer<int>("Obj Num");
        int maxObjects = objects->size();
    
	if (GUI.getBindingValue<bool>("Window Editor Open") == true) {
		ImGui::Begin("Editor", GUI.getBindingPointer<bool>("Window Editor Open"), ImGuiWindowFlags_NoCollapse);
        ImGui::InputInt("Obj #", objNum);

        *objNum = *objNum > maxObjects-1 ? 0 : *objNum;
        *objNum = *objNum < 0 ? maxObjects-1 : *objNum;

        std::shared_ptr<nova_Object> obj = objects->at(*objNum);
        ImGui::Text("Object Rotation:");
        ImGui::SliderFloat("X Rotation", &obj->transform.rotation.x, 0, 2*M_PI);
        ImGui::SliderFloat("Y Rotation", &obj->transform.rotation.y, 0, 2*M_PI);
        ImGui::SliderFloat("Z Rotation", &obj->transform.rotation.z, 0, 2*M_PI);
        ImGui::Text("Object Translation:");
        ImGui::SliderFloat("X Translation", &obj->transform.translation.x, min_dist_edit, max_dist_edit);
        ImGui::SliderFloat("Y Translation", &obj->transform.translation.y, min_dist_edit, max_dist_edit);
        ImGui::SliderFloat("Z Translation", &obj->transform.translation.z, min_dist_edit, max_dist_edit);
    	ImGui::End();
	}};

    }
}