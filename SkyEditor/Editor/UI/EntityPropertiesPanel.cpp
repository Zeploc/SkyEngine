#include "EntityPropertiesPanel.h"

#include <string>

#include "EditorApp.h"
#include "imgui_internal.h"
#include "Core/Asset/Asset.h"
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
	StartingSize = {430, 450};
	// TODO: Make button to open and remove
	bCanClose = false;
}

void CEntityPropertiesPanel::OnRender()
{
	// TODO: Convert to widget and use base render
	const TPointer<Entity> SelectedEntity = EditorApp->EditorViewportLayer->GetSelectedEntity();
    
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

void CEntityPropertiesPanel::DrawMeshComponent(const std::shared_ptr<CMeshComponent>& MeshComponent)
{
	ImGui::Text("Mesh Component");
	ImGui::Separator();
	ImGui::Spacing();
        
	ImGui::Text("Vao: %i", MeshComponent->GetVao());
	ImGui::Spacing();
	
	ImGui::Text("Mesh:");
	ImGui::SameLine();
	MeshDropdown(MeshComponent);
	ImGui::Spacing();	
	
	ImGui::Text("Material:");
	// ImGui::SameLine();
	MaterialsDropdown(MeshComponent);
	ImGui::Spacing();
        
	ImGui::Separator();
}

void CEntityPropertiesPanel::MaterialsDropdown(const std::shared_ptr<CMeshComponent>& MeshComponent)
{
    ImGuiContext& g = *GImGui;
	
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
	ImGui::BeginGroup();
	
	if (ImGui::Button("Image", ImVec2(60, 60)))
	{
		// Open combo
	}
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {10,10});
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {10,10});
		
	if (ImGui::BeginCombo("##MaterialsDropdown", MaterialName.c_str(), ImGuiComboFlags_None))
	{
	    // Display items
		// FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
		bool bValueChanged = false;

	    const TArray<TPointer<CAsset>> Materials = GetAssetManager()->GetAssetsOfClass(CMaterialInterface::GetStaticName());
		for (int i = 0; i < Materials.size(); i++)
		{
			TPointer<CAsset> MaterialAsset = Materials[i];
			ImGui::PushID(i);
			ImGui::BeginGroup();

			const bool bItemSelected = (MaterialAsset == CurrentMaterial->Asset);
			ImGui::BeginDisabled(bItemSelected);
			if (ImGui::Button("Image", ImVec2(60, 60)))
			{
				bValueChanged = true;
				TPointer<CMaterialInterface> LoadedMaterial = MaterialAsset->Load<CMaterialInterface>();
				MeshComponent->SetMaterial(LoadedMaterial);
				CurrentMaterial = LoadedMaterial;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			// if (!items_getter(data, i, &item_text))
			// 	item_text = "*Unknown item*";
			if (ImGui::Selectable(MaterialAsset->DisplayName.c_str(), bItemSelected))
			{
				bValueChanged = true;
				TPointer<CMaterialInterface> LoadedMaterial = MaterialAsset->Load<CMaterialInterface>();
				MeshComponent->SetMaterial(LoadedMaterial);
				CurrentMaterial = LoadedMaterial;
			}
			ImGui::EndDisabled();
			if (bItemSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
			ImGui::EndGroup();
			ImGui::PopID();
		}

		ImGui::EndCombo();	
		if (bValueChanged)
		{
			ImGui::MarkItemEdited(g.LastItemData.ID);
		}
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	
	//ImGui::Button("Drop Material", ImVec2(-1, 60));/*, ImVec2(60, 60));*/

	ImGui::EndGroup();
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(std::format("ASSET:{}", CMaterialInterface::GetStaticName()).c_str()))
		{
			IM_ASSERT(payload->DataSize == sizeof(TPointer<CAsset>));
			TPointer<CAsset> PayloadAsset = *(const TPointer<CAsset>*)payload->Data;
			CurrentMaterial = PayloadAsset->Load<CMaterialInterface>();
			MeshComponent->SetMaterial(CurrentMaterial);
		}
		ImGui::EndDragDropTarget();
	}
}

void CEntityPropertiesPanel::MeshDropdown(const std::shared_ptr<CMeshComponent>& MeshComponent)
{
	static std::string CurrentMeshName = "No Mesh";
	CurrentMeshName.reserve(50);
	std::string CurrentMeshAsset = MeshComponent->GetMeshAsset();
	if (!CurrentMeshAsset.empty())
	{
		CurrentMeshName = CurrentMeshAsset;
	}
	
	ImGuiContext& g = *GImGui;	

	if (!ImGui::BeginCombo("##MeshDropdown", CurrentMeshName.c_str(), ImGuiComboFlags_None))
	{
		return;
	}

	// Display items
	// FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
	bool bValueChanged = false;

	const TArray<std::string> MeshNames = GetMeshManager()->GetAvailableMeshes();
	for (int i = 0; i < MeshNames.size(); i++)
	{
		std::string MeshName = MeshNames[i];
		ImGui::PushID(i);
		const bool bItemSelected = (MeshName == CurrentMeshName);
		// if (!items_getter(data, i, &item_text))
		// 	item_text = "*Unknown item*";
		if (ImGui::Selectable(MeshName.c_str(), bItemSelected))
		{
			if (GetMeshManager()->HasMesh(MeshName))
			{
				MeshComponent->SetMeshAsset(MeshName);
				CurrentMeshName = MeshName;
				bValueChanged = true;
			}
		}
		if (bItemSelected)
		{
			ImGui::SetItemDefaultFocus();
		}
		ImGui::PopID();
	}

	ImGui::EndCombo();

	if (bValueChanged)
	{
		ImGui::MarkItemEdited(g.LastItemData.ID);
	}
}
