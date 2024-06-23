// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "ConfigSettingsPanel.h"

#include "UIPresets.h"
#include "Core/Application.h"
#include "Platform/Config/Config.h"
#include "Dependencies/ImGui/imgui.h"
#include "Render/Textures/Texture.h"

CConfigSettingsPanel::CConfigSettingsPanel(TWeakPointer<CEngineWindow> InOwningWindow, TPointer<CConfig> InConfig)
    : CUICanvas(InOwningWindow, "Config: " + InConfig->GetName()), Config((InConfig))
{
	StartingSize = {830, 450};
}

void CConfigSettingsPanel::OnRender()
{
	TArray<SConfigVariable> ConfigVariables = Config->GetVariables();
	for (SConfigVariable ConfigVariable : ConfigVariables)
	{
		CUIPresets::RenderVariableField(ConfigVariable);
	}
	ImGui::Spacing();
	if (ImGui::Button("Save"))
	{
		Config->SaveConfig();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reload from disk"))
	{
		Config->LoadConfig();
	}

	// TODO: Revert? Store original changes
	// TODO: Later open config file
}
