#include "ContentBrowser.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "MaterialEditorPanel.h"
#include "Core/Application.h"
#include "Core/StringUtils.h"
#include "Core/Asset/Asset.h"
#include "Platform/File/PathUtils.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Materials/Material.h"

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
	
	if (ImGui::Button("Back", {40,20}))
	{
		if (!Directory.empty())
		{
			Directory = Directory.substr(0, Directory.length() - 1);
		}
		Directory = PathUtils::GetDirectory(Directory);
		return;
	}

	TArray<std::string> Folders;
	std::string NewDirectory = Directory;

	// TODO: Do better
	bool bFirstInList = true;
	TArray<TPointer<CAsset>> Assets = GetAssetManager()->GetAssets();
	for (uint16 i = 0; i < Assets.size(); ++i)
	{
		TPointer<CAsset> Asset = Assets[i];

		std::string AssetPath = StringUtils::NormalizePath(Asset->FilePath);
		// Check asset is somewhere within this directory
		if (!Directory.empty() && !PathUtils::IsRelativeToPath(AssetPath, Directory))
		{
			continue;
		}		

		const std::string AssetDirectory = PathUtils::GetDirectory(AssetPath);
		const std::string RelativeAssetDirectory = Directory.empty() ? AssetDirectory : PathUtils::GetRelativePath(AssetDirectory, Directory);
		// If asset is not in the current folder
		if (!RelativeAssetDirectory.empty())
		{
			std::string FolderName = StringUtils::Split(RelativeAssetDirectory, "\\")[0];
			if (Utils::ArrayContains(Folders, FolderName))
			{
				continue;
			}
			
			// ImGui::PushID(i);
			if ((i % NumberPerRow) != 0 && !bFirstInList)
				ImGui::SameLine();
			
			bFirstInList = false;

			Folders.push_back(FolderName);
			
			// Create Folder button
			if (ImGui::Button(FolderName.c_str(), {65,65}))
			{
				NewDirectory = (Directory.empty() ? FolderName : PathUtils::CombinePath(Directory, FolderName)) + "\\";
			}
			continue;
		}
		
		ImGui::PushID(i);
		if ((i % NumberPerRow) != 0 && !bFirstInList)
			ImGui::SameLine();

		bFirstInList = false;

		// ImGui::PushID(i+100);
		// ImGui::ButtonEx(std::format("##%s", Asset->FilePath).c_str());
		if (ImGui::Button(std::format("{}\n{}", Asset->DisplayName, Asset->ClassName).c_str(), {65,90}))
		{
			Asset->Open();
			// TODO: Determine structure for opening at a per asset basis without having editor code in engine...
			if (TPointer<CMaterialInterface> Material = Asset->Load<CMaterialInterface>())
			{
				GetApplication()->GetApplicationWindow()->GetCanvas<CMaterialEditorPanel>()->SetMaterial(Material);
			}
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
			ImGui::TextUnformatted(AssetPath.c_str());
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
			ImGui::Text(AssetPath.c_str());
			ImGui::EndDragDropSource();
		}
		// ImGui::EndChild();
		// ImGui::PopID();
		// ImGui::EndGroup();
		
		ImGui::PopID();
	}
	Directory = NewDirectory;
}
