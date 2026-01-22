#include "EntityPropertiesPanel.h"

#include <string>

#include "EditorApp.h"
#include "imgui_internal.h"
#include "UIPresets.h"
#include "Dependencies/ImGui/imgui.h"
#include "Editor/EditorViewportCanvas.h"
#include "Entity/Entity.h"

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
	const TSharedPointer<Entity> SelectedEntity = EditorApp->EditorViewportLayer->GetSelectedEntity();
    
	if (SelectedEntity)
	{
		ImGui::Text(SelectedEntity->GetEntityName().c_str());
		ImGui::Separator();
		ImGui::Spacing();

		for (SSerializableVariable& SerializableVariable : SelectedEntity->GetSerializeVariables())
		{
			CUIPresets::RenderVariableField(SerializableVariable);
		}
        
		ImGui::Separator();

		// TODO: Switch to use serialize variables on components
		for (const TSharedPointer<CComponent>& Component : SelectedEntity->GetComponents())
		{			
			ImGui::Text(Component->GetComponentClassName().c_str());
			ImGui::Separator();
			ImGui::Spacing();
			
			for (SSerializableVariable& SerializableVariable : Component->GetSerializeVariables())
			{
				CUIPresets::RenderVariableField(SerializableVariable);
			}
		}
	}
}

void CEntityPropertiesPanel::MeshDropdown(const std::shared_ptr<CMeshComponent>& MeshComponent)
{
	// TODO: Swap with asset field once mesh is an asset
	// static std::string CurrentMeshName = "No Mesh";
	// CurrentMeshName.reserve(50);
	// std::string CurrentMeshAsset = MeshComponent->GetMeshAsset();
	// if (!CurrentMeshAsset.empty())
	// {
	// 	CurrentMeshName = CurrentMeshAsset;
	// }
	//
	// ImGuiContext& g = *GImGui;	
	//
	// if (!ImGui::BeginCombo("##MeshDropdown", CurrentMeshName.c_str(), ImGuiComboFlags_None))
	// {
	// 	return;
	// }
	//
	// // Display items
	// // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
	// bool bValueChanged = false;
	//
	// const TArray<std::string> MeshNames = GetMeshManager()->GetAvailableMeshes();
	// for (int i = 0; i < MeshNames.size(); i++)
	// {
	// 	std::string MeshName = MeshNames[i];
	// 	ImGui::PushID(i);
	// 	const bool bItemSelected = (MeshName == CurrentMeshName);
	// 	// if (!items_getter(data, i, &item_text))
	// 	// 	item_text = "*Unknown item*";
	// 	if (ImGui::Selectable(MeshName.c_str(), bItemSelected))
	// 	{
	// 		if (GetMeshManager()->HasMesh(MeshName))
	// 		{
	// 			// MeshComponent->SetMeshAsset(MeshName);
	// 			CurrentMeshName = MeshName;
	// 			bValueChanged = true;
	// 		}
	// 	}
	// 	if (bItemSelected)
	// 	{
	// 		ImGui::SetItemDefaultFocus();
	// 	}
	// 	ImGui::PopID();
	// }
	//
	// ImGui::EndCombo();
	//
	// if (bValueChanged)
	// {
	// 	ImGui::MarkItemEdited(g.LastItemData.ID);
	// }
}
