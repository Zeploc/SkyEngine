#include "EntityPropertiesPanel.h"

#include <string>

#include "EditorApp.h"
#include "Core/String.h"
#include "Dependencies/ImGui/imgui.h"
#include "Editor/EditorViewportCanvas.h"
#include "Editor/Scene/EditorScene.h"
#include "Entity/Entity.h"
#include "Render/Materials/InternalMaterial.h"
#include "Scene/SceneManager.h"
#include "System/LogManager.h"

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

		for (const TPointer<CComponent>& Component : SelectedEntity->GetComponents())
		{
			if (TPointer<CMeshComponent> MeshComponent = Cast<CMeshComponent>(Component))
			{
				DrawMeshComponent(MeshComponent);
			}
		}
	}
}

int CEntityPropertiesPanel::MaterialTextChanged(ImGuiInputTextCallbackData* data)
{
	CMeshComponent* MeshComponent = static_cast<CMeshComponent*>(data->UserData);
	const std::string InputtedName = data->Buf;
	if (const TPointer<CMaterialInterface> FoundMaterial = FindMaterial(InputtedName))
	{
		MeshComponent->SetMaterial(FoundMaterial);
	}
	return 0;
}

int CEntityPropertiesPanel::MeshTextChanged(ImGuiInputTextCallbackData* data)
{
	CMeshComponent* MeshComponent = static_cast<CMeshComponent*>(data->UserData);
	const std::string InputtedName = data->Buf;
	if (!InputtedName.empty())
	{
		MeshComponent->SetMeshAsset(InputtedName);
	}
	return 0;
}

void CEntityPropertiesPanel::DrawMeshComponent(const std::shared_ptr<CMeshComponent>& MeshComponent)
{
	ImGui::Text("Mesh Component");
	ImGui::Separator();
	ImGui::Spacing();
        
	ImGui::Text("Vao: %i", MeshComponent->GetVao());
	ImGui::Spacing();
	
	static std::string MeshName = "No Mesh";
	MeshName.reserve(50);
	std::string CurrentMeshAsset = MeshComponent->GetMeshAsset();
	if (!CurrentMeshAsset.empty())
	{
		MeshName = CurrentMeshAsset;
	}
	ImGui::Text("Mesh:");
	ImGui::SameLine();
	ImGui::InputText("##MeshName", (char*)MeshName.c_str(), MeshName.capacity() + 1, ImGuiInputTextFlags_CallbackEdit, MeshTextChanged, MeshComponent.get());
	ImGui::Spacing();	

	static std::string MaterialName = "No Material";
	MaterialName.reserve(50);
	TPointer<CMaterialInterface> CurrentMaterial = MeshComponent->GetMaterial();
	if (CurrentMaterial)
	{
		MaterialName = CurrentMaterial->GetMaterialName();
	}
	else
	{
		MaterialName = "No Material";
	}
	
	ImGui::Text("Material:");
	ImGui::SameLine();
	ImGui::InputText("##MeshMaterial", (char*)MaterialName.c_str(), MaterialName.capacity() + 1, ImGuiInputTextFlags_CallbackEdit, MaterialTextChanged, MeshComponent.get());
	ImGui::Spacing();	
        
	ImGui::Separator();
}


TPointer<CMaterialInterface> CEntityPropertiesPanel::FindMaterial(std::string MaterialName)
{
	for (TPointer<Entity> Entity : SceneManager::GetInstance()->GetCurrentScene()->Entities)
	{
		const TPointer<CMeshComponent> MeshComponent = Entity->FindComponent<CMeshComponent>();
		if (!MeshComponent)
		{
			continue;
		}
		TPointer<CMaterialInterface> MeshMaterial = MeshComponent->GetMaterial();
		if (!MeshMaterial)
		{
			continue;
		}
		std::string MeshMaterialName = MeshMaterial->GetMaterialName();
		if (MeshMaterialName == MaterialName)
		{
			return MeshMaterial;
		}
	}
	return nullptr;
}
