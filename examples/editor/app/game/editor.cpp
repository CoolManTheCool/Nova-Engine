#include "editor.hpp"
#include "gui_system.hpp"
#include "objects/object.hpp"
#include "objects/mesh_object.hpp"

namespace Nova {

// Min and max translation distances for editor sliders
#define min_dist_edit -10
#define max_dist_edit 10

Editor_T::Editor_T() {

}

void Editor_T::RegisterBindings() {
    GUI.setBinding("Window Editor Open", true);
    GUI.setBinding("Obj Num", 0);
}

void Editor_T::RegisterWindow(Window* window) {

    std::vector<std::shared_ptr<Object>> *objects = GUI.getBindingValue<std::vector<std::shared_ptr<Object>>*>("Objects");
    assert(objects && "'Objects' is not bound or is null.");
    
    GUI.registerWindow([window, objects]() {
        int* objNum = GUI.getBindingPointer<int>("Obj Num");
        int maxObjects = objects->size();
    
	if (GUI.getBindingValue<bool>("Window Editor Open") == true) {
		ImGui::Begin("Editor", GUI.getBindingPointer<bool>("Window Editor Open"), ImGuiWindowFlags_NoCollapse);
        ImGui::InputInt("Obj #", objNum);

        *objNum = *objNum > maxObjects-1 ? 0 : *objNum;
        *objNum = *objNum < 0 ? maxObjects-1 : *objNum;

        std::shared_ptr<Object> obj = objects->at(*objNum);
        ImGui::Text("Object Rotation:");
        ImGui::SliderFloat("X Rotation", &obj->transform.rotation.x, 0, 2*M_PI);
        ImGui::SliderFloat("Y Rotation", &obj->transform.rotation.y, 0, 2*M_PI);
        ImGui::SliderFloat("Z Rotation", &obj->transform.rotation.z, 0, 2*M_PI);
        ImGui::Text("Object Translation:");
        ImGui::SliderDouble("X Translation", &obj->transform.translation.x, min_dist_edit, max_dist_edit);
        ImGui::SliderDouble("Y Translation", &obj->transform.translation.y, min_dist_edit, max_dist_edit);
        ImGui::SliderDouble("Z Translation", &obj->transform.translation.z, min_dist_edit, max_dist_edit);
        // Is obj a mesh, if so conver it
        if (obj->getObjectType() == OBJECT_TYPE_MESH) {
            auto meshObj = std::dynamic_pointer_cast<Nova::MeshObject>(obj);
            ImGui::SeparatorText("Mesh Properties");
            ImGui::SliderFloat("Roughness", &meshObj->roughness, 0.0f, 1.0f);
        }
    	ImGui::End();
	}});

    }
}