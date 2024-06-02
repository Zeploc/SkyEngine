#include "ContentBrowser.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Application.h"
#include "Core/Asset/Asset.h"

CContentBrowser::CContentBrowser(const TWeakPointer<CEngineWindow>& InOwningWindow): CUICanvas(InOwningWindow, "Content Browser")
{
	StartingSize = {800, 300};
	// TODO: Make button to open and remove
	bCanClose = false;
}

void CContentBrowser::OnRender()
{
	const int NumberPerRow = floor(ImGui::GetWindowWidth() / (60.0f + ImGui::GetStyle().CellPadding.x * 2.0f));

	// TODO: Multiple selection
	// if (ImGui::TreeNode("Selection State: Multiple Selection"))
	// {
	// 	static bool selection[5] = { false, false, false, false, false };
	// 	for (int n = 0; n < 5; n++)
	// 	{
	// 		char buf[32];
	// 		sprintf(buf, "Object %d", n);
	// 		if (ImGui::Selectable(buf, selection[n]))
	// 		{
	// 			if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
	// 				memset(selection, 0, sizeof(selection));
	// 			selection[n] ^= 1;
	// 		}
	// 	}
	// 	ImGui::TreePop();
	// }

	TArray<TPointer<CAsset>> Assets = GetAssetManager()->GetAssets();
	for (int i = 0; i < Assets.size(); ++i)
	{
		TPointer<CAsset> Asset = Assets[i];
		ImGui::PushID(i);
		if ((i % NumberPerRow) != 0)
			ImGui::SameLine();

		// ImGui::PushID(i+100);
		// ImGui::ButtonEx(std::format("##%s", Asset->FilePath).c_str());
		if (ImGui::Button(std::format("{}\n{}", Asset->DisplayName, Asset->ClassName).c_str(), {65,90}))
		{
			Asset->Open();
		}
		// ImGui::BeginChild(i + 100, ImVec2(65, 100), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		//ImGui::ImageButton(Asset->DisplayName.c_str(), nullptr, ImVec2(60, 60));
		// ImGui::BeginGroup();//std::format("##%s2", Asset->FilePath).c_str(), nullptr);
		// ImGui::Selectable(std::format("##%s", Asset->FilePath).c_str(), false, 0, {65, 100});
		// ImGui::Image(nullptr, ImVec2(60, 60));
		// ImGui::Text("%s", Asset->DisplayName.c_str());
		// ImGui::Text("%s", Asset->ClassName.c_str());
		// ImGui::GetWindowDrawList()->AddText({0,0}, 0, "test");
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(Asset->DisplayName.c_str());
			ImGui::TextUnformatted(Asset->ClassName.c_str());
			for (std::string Data : Asset->Metadata)
			{
				ImGui::TextUnformatted(Data.c_str());
			}
			ImGui::TextUnformatted(Asset->FilePath.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		// Our buttons are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the index of our item (could be anything)
			// TODO: Need to split up material into meta data for shader type, so class type is just material
			ImGui::SetDragDropPayload(std::format("ASSET:{}", Asset->ClassName).c_str(), &Asset, sizeof(TPointer<CAsset>));

			// Display preview (could be anything, e.g. when dragging an image we could decide to display
			// the filename and a small preview of the image, etc.)
			// TODO: Change to asset name/path
			ImGui::Text(Asset->FilePath.c_str());
			ImGui::EndDragDropSource();
		}
		// ImGui::EndChild();
		// ImGui::PopID();
		// ImGui::EndGroup();
		
		ImGui::PopID();
	}
}
