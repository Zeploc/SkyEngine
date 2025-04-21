// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "UIPresets.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Application.h"
#include "Core/SerializableVariable.h"
#include "Core/Asset/Asset.h"
#include "Render/Textures/Texture.h"
#include "System/LogManager.h"

// TODO: Vector int vs Vector float + UI ranges? (min, max) + String dropdown/options + 
void CUIPresets::RenderVariableField(SSerializableVariable& Variable)
{
	bool bVariableChanged = false;
	switch (Variable.Type)
	{
	case EVariableType::None:
		break;
	case EVariableType::Boolean:
		{
			const bool bOriginalValue = *Variable.Boolean;
			bVariableChanged = ImGui::Checkbox(Variable.VariableName.c_str(), Variable.Boolean);
			if (Variable.HasMetaTag(MetaTag_ReadOnlyEditor))
			{
				*Variable.Boolean = bOriginalValue;
			}
		}
		break;
	case EVariableType::String:
		{
            char str0[128];
			sprintf_s(str0,"%s", Variable.String->c_str());
			ImGuiInputFlags Flags = 0;
			if (Variable.HasMetaTag(MetaTag_ReadOnlyEditor))
			{
				Flags |= ImGuiInputTextFlags_ReadOnly;
			}
			bVariableChanged = ImGui::InputText(Variable.VariableName.c_str(), str0, IM_ARRAYSIZE(str0), Flags);
			*Variable.String = str0;
		}
		break;
	case EVariableType::Number:
		{
			ImGuiSliderFlags Flags = 0;
			if (Variable.HasMetaTag(MetaTag_ReadOnlyEditor))
			{
				Flags |= ImGuiSliderFlags_ReadOnly;
			}
			bVariableChanged = ImGui::DragScalarN(Variable.VariableName.c_str(), ImGuiDataType_Float, Variable.Number, 1, 0.2f, nullptr, nullptr, "%.2f", Flags);
		}
		break;
	case EVariableType::Integer:
		{
			ImGuiSliderFlags Flags = 0;
			if (Variable.HasMetaTag(MetaTag_ReadOnlyEditor))
			{
				Flags |= ImGuiSliderFlags_ReadOnly;
			}
			bVariableChanged = ImGui::DragScalarN(Variable.VariableName.c_str(), ImGuiDataType_U32, Variable.Integer, 1, 1, nullptr, nullptr, nullptr, Flags);
		}
		break;
		// TODO: Support int and float versions of below
	case EVariableType::Vector2:
		{
			ImGuiSliderFlags Flags = 0;
			if (Variable.HasMetaTag(MetaTag_ReadOnlyEditor))
			{
				Flags |= ImGuiSliderFlags_ReadOnly;
			}
			SVector2* Vector = Variable.Vector2;
        
			uint32_t Position[2] = { (uint32_t)Vector->X, (uint32_t)Vector->Y};
			// ImGui::DragFloat3("Position", Position, 0.01f, 0.0f, 1.0f);
			bVariableChanged = ImGui::DragScalarN(Variable.VariableName.c_str(), ImGuiDataType_U32, Position, 2, 10.0f, nullptr, nullptr, nullptr, Flags);
			Vector->X = Position[0];
			Vector->Y = Position[1];
		}
		break;
	case EVariableType::Vector4:
		{
			ImGuiSliderFlags Flags = 0;
			if (Variable.HasMetaTag(MetaTag_ReadOnlyEditor))
			{
				Flags |= ImGuiSliderFlags_ReadOnly;
			}
			SVector4* Vector = Variable.Vector4;
        
			float Position[4] = { Vector->X, Vector->Y, Vector->Z, Vector->W};
			bVariableChanged = ImGui::DragScalarN(Variable.VariableName.c_str(), ImGuiDataType_Float, Position, 4, 0.1f, nullptr, nullptr, nullptr, Flags);
			Vector->X = Position[0];
			Vector->Y = Position[1];
			Vector->Z = Position[2];
			Vector->W = Position[3];
		}
		break;
	case EVariableType::Transform:
		{        
			ImGuiSliderFlags Flags = 0;
			if (Variable.HasMetaTag(MetaTag_ReadOnlyEditor))
			{
				Flags |= ImGuiSliderFlags_ReadOnly;
			}
			
			STransform& Transform = *Variable.Transform;

			// TODO: Create vector 3 and 4 UI presets and use those instead (also use above)
			float Position[3] = { Transform.Position.x, Transform.Position.y, Transform.Position.z};
			// ImGui::DragFloat3("Position", Position, 0.01f, 0.0f, 1.0f);
			bVariableChanged = ImGui::DragScalarN("Position", ImGuiDataType_Float, Position, 3, 0.05f, nullptr, nullptr, "%.3f", Flags);
			Transform.Position.x = Position[0];
			Transform.Position.y = Position[1];
			Transform.Position.z = Position[2];
			ImGui::Spacing();
        
			float Rotation[3] = { Transform.Rotation.Yaw, Transform.Rotation.Pitch, Transform.Rotation.Roll};
			bVariableChanged = ImGui::DragScalarN("Rotation", ImGuiDataType_Float, Rotation, 3, 0.2f, nullptr, nullptr, "%.2f", Flags);
			Transform.Rotation.Yaw = Rotation[0];
			Transform.Rotation.Pitch = Rotation[1];
			Transform.Rotation.Roll = Rotation[2];
			ImGui::Spacing();
        
			float Scale[3] = { Transform.Scale.x, Transform.Scale.y, Transform.Scale.z};
			bVariableChanged = ImGui::DragScalarN("Scale", ImGuiDataType_Float, Scale, 3, 0.005f, nullptr, nullptr, "%.2f", Flags);
			Transform.Scale.x = Scale[0];
			Transform.Scale.y = Scale[1];
			Transform.Scale.z = Scale[2];
			ImGui::Spacing();
		}
		break;
	case EVariableType::Object:
		{
			// TODO: Read only option
			bVariableChanged = RenderAssetObjectField(*Variable.AssetObject, Variable.GetMetaTag(MetaTag_Class));	
		}
		break;
	default: ;
	}
	if (bVariableChanged)
	{
		CLogManager::Get()->DisplayMessage("Value changed!");
		Variable.OnValueChanged();
	}
}

bool CUIPresets::RenderAssetObjectField(TAssetObjectPointer<>& AssetObject, std::string ClassFilter)
{
    ImGuiContext& g = *GImGui;

	TObjectPointer<CAsset> Asset = nullptr;
	static std::string AssetName;
	AssetName.reserve(50);
	if (AssetObject)
	{
		Asset = AssetObject->Asset;
		AssetName = Asset->DisplayName;
	}
	else
	{
		AssetName = "No Asset";
	}
	
	ImGui::BeginGroup();
	
	if (ImGui::Button(ClassFilter.c_str(), ImVec2(60, 60)))
	{
		// Open combo
	}
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {10,10});
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {10,10});
		
	bool bValueChanged = false;
	// TODO: Should use property name instead
    const std::string DropdownName = "##" + ClassFilter + "Dropdown";
	if (ImGui::BeginCombo(DropdownName.c_str(), AssetName.c_str(), ImGuiComboFlags_None))
	{
	    // Display items
		// FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)

	    TArray<TObjectPointer<CAsset>> Assets = ClassFilter.empty() ? GetAssetManager()->GetAssets() : GetAssetManager()->GetAssetsOfClass(ClassFilter);
		Assets.insert(Assets.begin(), nullptr);
		for (uint32_t i = 0; i < Assets.size(); i++)
		{
			const TObjectPointer<CAsset> CurrentAsset = Assets[i];
			ImGui::PushID((int)i);
			ImGui::BeginGroup();

			const bool bItemSelected = (CurrentAsset == Asset);
			ImGui::BeginDisabled(bItemSelected);
			if (ImGui::Button("Image", ImVec2(60, 60)))
			{
				Asset = CurrentAsset;
				bValueChanged = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			// if (!items_getter(data, i, &item_text))
			// 	item_text = "*Unknown item*";
			if (ImGui::Selectable(CurrentAsset ? CurrentAsset->DisplayName.c_str() : "None", bItemSelected))
			{
				Asset = CurrentAsset;
				bValueChanged = true;
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
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(std::format("ASSET:{}", ClassFilter).c_str()))
		{
			IM_ASSERT(payload->DataSize == sizeof(THardPointer<CAsset>));
			Asset = *(const THardPointer<CAsset>*)payload->Data;
			bValueChanged = true;
		}
		ImGui::EndDragDropTarget();
	}
	if (bValueChanged)
	{
		if (Asset)
		{
			AssetObject = Asset->Load();
		}
		else
		{
			AssetObject = nullptr;
		}
	}
	return bValueChanged;
}