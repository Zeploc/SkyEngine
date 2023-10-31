#include "EntityPropertiesPanel.h"

#include <string>

#include "EditorApp.h"
#include "Dependencies/ImGui/imgui.h"
#include "Editor/EditorViewportCanvas.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/Scene/TransformationWidget.h"
#include "Scene/SceneManager.h"

CEntityPropertiesPanel::CEntityPropertiesPanel(TWeakPointer<CEngineWindow> InOwningWindow)
    : CUICanvas(InOwningWindow, "Entity: Property editor")
{
	// TODO: Remove once redundant
    EditorScene = std::static_pointer_cast<class EditorScene>(SceneManager::GetInstance()->GetCurrentScene());
	StartingSize = {430, 450};
}

void CEntityPropertiesPanel::OnRender()
{
	// TODO: Convert to widget and use base render
	TPointer<Entity> SelectedEntity = EditorApp->EditorViewportLayer->GetSelectedEntity();
    
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
        
		float Rotation[3] = { Transform.Rotation.Yaw, Transform.Rotation.Pitch, Transform.Rotation.Roll};
		ImGui::DragScalarN("Rotation", ImGuiDataType_Float, Rotation, 3, 0.2f, nullptr, nullptr, "%.2f");
		Transform.Rotation.Yaw = Rotation[0];
		Transform.Rotation.Pitch = Rotation[1];
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
}
