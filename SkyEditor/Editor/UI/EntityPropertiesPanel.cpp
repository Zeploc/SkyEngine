#include "EntityPropertiesPanel.h"

#include <string>

#include "Dependencies/ImGui/imgui.h"

void CEntityPropertiesPanel::DrawUI()
{
    static bool bOpen = true;
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Entity: Property editor", &bOpen))
    {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
    ImGui::Columns(2);
    ImGui::Separator();

    int uid = 0;
    ImGui::PushID(uid);                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
    
    static std::string names[9] = {
        "Pos X","Pos Y","Pos Z",
        "Rot X","Rot Y","Rot Z",
        "Scale X","Scale Y","Scale Z",
    };
    static float Members[9] = { 0.0f,0.0f,10.0f,90.0f,0.0f,180.0f,1.0f,1.0f,1.0f };
    for (int i = 0; i < 9; i++)
    {
        ImGui::PushID(i); // Use field index as identifier.
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, names[i].c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        float Speed = 0.05f;
        if (i >= 6)
        {
            Speed = 0.005f;
        }
        else if (i >= 3)
        {
            Speed = 0.2f;
        }
        ImGui::DragFloat("##value", &Members[i], Speed);
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        ImGui::PopID();
    }
    ImGui::PopID();

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
}
