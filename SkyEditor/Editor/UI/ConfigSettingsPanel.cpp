#include "ConfigSettingsPanel.h"
#include "Platform/Config/Config.h"
#include "Dependencies/ImGui/imgui.h"

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
		RenderVariableField(ConfigVariable);
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

void CConfigSettingsPanel::RenderVariableField(SConfigVariable ConfigVariable)
{
	switch (ConfigVariable.Type)
	{
	case EVariableType::None:
		break;
	case EVariableType::Boolean:
		{
			ImGui::Checkbox(ConfigVariable.VariableName.c_str(), ConfigVariable.Boolean);
		}
		break;
	case EVariableType::String:
		{
            char str0[128];
			sprintf_s(str0,"%s", ConfigVariable.String->c_str());
			ImGui::InputText(ConfigVariable.VariableName.c_str(), str0, IM_ARRAYSIZE(str0));
			*ConfigVariable.String = str0;
		}
		break;
	case EVariableType::Number:
		{
			ImGui::DragScalarN(ConfigVariable.VariableName.c_str(), ImGuiDataType_Float, ConfigVariable.Number, 1, 0.2f, nullptr, nullptr, "%.2f");
		}
		break;
	case EVariableType::Integer:
		{
			ImGui::DragScalarN(ConfigVariable.VariableName.c_str(), ImGuiDataType_U32, ConfigVariable.Integer, 1, 1, nullptr, nullptr);
		}
		break;
	case EVariableType::Vector2:
		{
			SVector2* Vector = ConfigVariable.Vector2;
        
			uint32_t Position[2] = { (uint32_t)Vector->X, (uint32_t)Vector->Y};
			// ImGui::DragFloat3("Position", Position, 0.01f, 0.0f, 1.0f);
			ImGui::DragScalarN(ConfigVariable.VariableName.c_str(), ImGuiDataType_U32, Position, 2, 10.0f, nullptr, nullptr);
			Vector->X = Position[0];
			Vector->Y = Position[1];
		}
		break;
	default: ;
	}
}
