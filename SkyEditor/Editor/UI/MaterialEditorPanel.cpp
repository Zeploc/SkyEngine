// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "MaterialEditorPanel.h"

#include "ConfigSettingsPanel.h"
#include "imgui_internal.h"
#include "UIPresets.h"
#include "Core/Asset/Asset.h"
#include "Render/Materials/Material.h"

CMaterialEditorPanel::CMaterialEditorPanel(TWeakPointer<CEngineWindow> InOwningWindow)
    : CUICanvas(InOwningWindow, "Material Editor")
{
	StartingSize = {400, 650};
}

void CMaterialEditorPanel::OnRender()
{
	if (!Material)
	{
		ImGui::Text("No material selected");
		return;
	}
	ImGui::Text(Material->GetMaterialName().c_str());
	TArray<SShaderParameter>& MaterialVariables = Material->GetMaterialVariables();
	for (SShaderParameter& ShaderParameter : MaterialVariables)
	{
		CUIPresets::RenderVariableField(ShaderParameter);
	}
	
	ImGui::Spacing();
	if (ImGui::Button("Save"))
	{
		Material->Asset->Save();
	}
	ImGui::SameLine();
	// TODO: Reload doesn't update existing references to old asset
	if (ImGui::Button("Reload from disk"))
	{
		Material = Cast<CMaterialInterface>(Material->Asset->Reload());
	}
}

void CMaterialEditorPanel::SetMaterial(TPointer<CMaterialInterface> InMaterial)
{
	Material = InMaterial;
}
