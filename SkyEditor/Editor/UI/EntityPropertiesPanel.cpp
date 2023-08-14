#include "EntityPropertiesPanel.h"

#include <string>

#include "EditorApp.h"
#include "Dependencies/ImGui/imgui.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/Scene/TransformationWidget.h"
#include "Scene/SceneManager.h"

CEntityPropertiesPanel::CEntityPropertiesPanel()
{
    EditorScene = std::static_pointer_cast<class EditorScene>(SceneManager::GetInstance()->GetCurrentScene());
}

void CEntityPropertiesPanel::DrawUI(const SCanvas& DrawCanvas)
{
    static bool bOpen = true;
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Entity: Property editor", &bOpen))
    {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));

    TPointer<Entity> SelectedEntity = EditorScene->TransformationWidget->SelectedEntity;

    if (SelectedEntity)
    {
        ImGui::Text(SelectedEntity->GetEntityName().c_str());
        ImGui::Separator();
        ImGui::Spacing();
        
        STransform& Transform = SelectedEntity->Transform;
        
        float Position[3] = { Transform.Position.x, Transform.Position.y, Transform.Position.z};
        // ImGui::DragFloat3("Position", Position, 0.01f, 0.0f, 1.0f);
        ImGui::DragScalarN("Position", ImGuiDataType_Float, Position, 3, 0.05f, nullptr, nullptr, "%.3f");
        Transform.Position.x = Position[0];
        Transform.Position.y = Position[1];
        Transform.Position.z = Position[2];
        ImGui::Spacing();
        
        float Rotation[3] = { Transform.Rotation.Pitch, Transform.Rotation.Yaw, Transform.Rotation.Roll};
        ImGui::DragScalarN("Rotation", ImGuiDataType_Float, Rotation, 3, 0.2f, nullptr, nullptr, "%.2f");
        Transform.Rotation.Pitch = Rotation[0];
        Transform.Rotation.Yaw = Rotation[1];
        Transform.Rotation.Roll = Rotation[2];
        ImGui::Spacing();
        
        float Scale[3] = { Transform.Scale.x, Transform.Scale.y, Transform.Scale.z};
        ImGui::DragScalarN("Scale", ImGuiDataType_Float, Scale, 3, 0.005f, nullptr, nullptr, "%.2f");
        Transform.Scale.x = Scale[0];
        Transform.Scale.y = Scale[1];
        Transform.Scale.z = Scale[2];
        ImGui::Spacing();
        
        ImGui::Separator();
    }
    
    ImGui::PopStyleVar();
    ImGui::End();
}
