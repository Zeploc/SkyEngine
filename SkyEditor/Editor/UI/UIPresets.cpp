// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "UIPresets.h"

#include "imgui.h"
#include "Core/Application.h"
#include "Core/SerializableVariable.h"
#include "Core/Asset/Asset.h"

// TODO: Vector int vs Vector float + UI ranges? (min, max) + String dropdown/options + 
void CUIPresets::RenderVariableField(SSerializableVariable ConfigVariable)
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
	case EVariableType::Vector4:
		{
			SVector4* Vector = ConfigVariable.Vector4;
        
			float Position[4] = { Vector->X, Vector->Y, Vector->Z, Vector->W};
			ImGui::DragScalarN(ConfigVariable.VariableName.c_str(), ImGuiDataType_Float, Position, 4, 0.1f, nullptr, nullptr);
			Vector->X = Position[0];
			Vector->Y = Position[1];
			Vector->Z = Position[2];
			Vector->W = Position[3];
		}
		break;
	case EVariableType::Asset:
		{
			// TODO: Dropdown like material
			char str0[128];
			std::string TexturePath = "";
			if (*ConfigVariable.Asset)
			{
				TexturePath = (*ConfigVariable.Asset)->FilePath;
			}
			sprintf_s(str0,"%s", TexturePath.c_str());
			ImGui::InputText(ConfigVariable.VariableName.c_str(), str0, IM_ARRAYSIZE(str0));
		
			*ConfigVariable.Asset = GetAssetManager()->FindAsset(str0);		
		}
		break;
	default: ;
	}
}