#include "ContentBrowser.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "MaterialEditorPanel.h"
#include "Core/Application.h"
#include "Core/StringUtils.h"
#include "Core/Asset/Asset.h"
#include "Platform/File/FileManager.h"
#include "Platform/File/PathUtils.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Materials/Material.h"
#include "Render/Meshes/Model/ModelMesh.h"
#include "Render/Shaders/PBRShader.h"
#include "Render/Shaders/UnlitShader.h"

CContentBrowser::CContentBrowser(const TWeakPointer<CEngineWindow>& InOwningWindow): CUICanvas(InOwningWindow, "Content Browser")
{
	StartingSize = {800, 300};
	// TODO: Make button to open and remove
	bCanClose = false;
}

void CContentBrowser::DeletePopup(TSharedPointer<CAsset> Asset)
{
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
	
	static char AssetName[128];
	static std::string NewAssetClassName;
	static TArray<std::string> MetaData;
	
	if (ImGui::BeginPopupContextWindow("EmptySpaceContextMenu"))
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::BeginMenu("Material"))
			{
				// TODO: Additional option for shader base (could be later changed in material asset but would change properties - same as editing a shader and recompiling though)
				if (ImGui::MenuItem("PBR"))
				{
					NewAssetClassName = CMaterial_PBR::GetStaticName();
					MetaData.push_back(CPBRShader::GetStaticName());
				}
				else if (ImGui::MenuItem("Unlit"))
				{
					NewAssetClassName = CMaterial_Unlit::GetStaticName();
					MetaData.push_back(CUnlitShader::GetStaticName());
				}				
					
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Texture"))
			{
				NewAssetClassName = CTexture::GetStaticName();
			}
			if (ImGui::MenuItem("Scene"))
			{
				NewAssetClassName = Scene::GetStaticName();
			}
			if (ImGui::MenuItem("Mesh"))
			{
				NewAssetClassName = CMesh::GetStaticName();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Refresh"))
		{
			GetAssetManager()->ScanForAssets();
			// TODO: Look for missing assets and prompt to remove or keep and warn if not saved
		}
		ImGui::EndPopup();
	}

	std::string MeshFilePath;
	TObjectPointer<CAsset> CreatedAsset;
	if (ImGui::BeginPopup("NamePopup"))
	{			
		ImGui::Text("Enter name");						
		// ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputText("##Name", AssetName, IM_ARRAYSIZE(AssetName));
		if (ImGui::Button("Cancel"))
		{
			NewAssetClassName = {};
			AssetName[0] = '\0';
			MetaData.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			bool bAdditionalStepComplete = true;
			std::string Extension = ".sasset";
			if (NewAssetClassName == Scene::GetStaticName())
			{
				Extension = ".slvl";
			}
			else if (NewAssetClassName == CMesh::GetStaticName())
			{
				if (!CFileManager::OpenFile(MeshFilePath, "obj", GetApplication()->GetContentDirectory()))
				{
					bAdditionalStepComplete = false;
				}
			}
			if (bAdditionalStepComplete)
			{
				CreatedAsset = GetAssetManager()->AddAsset(Directory + std::string(AssetName) + Extension, NewAssetClassName);
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (CreatedAsset)
	{
		CreatedAsset->Metadata = MetaData;
		TSharedPointer<CAssetObject> NewAsset = CreatedAsset->MakeObject();
		if (!MeshFilePath.empty())
		{
			Cast<CMesh>(NewAsset)->MeshPath = MeshFilePath;
		}
		CreatedAsset->SetDefaultObject(NewAsset);
		CreatedAsset->Load()->OnLoaded();
		CreatedAsset->Save();
			
		NewAssetClassName = {};
		AssetName[0] = '\0';
		MetaData.clear();
	}
	
	if (!NewAssetClassName.empty())
	{
		ImGui::OpenPopup("NamePopup");
		if (AssetName[0] == '\0')
		{
			sprintf_s(AssetName,"New%s", NewAssetClassName.c_str());
		}
	}
	
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
	TArray<TObjectPointer<CAsset>> Assets = GetAssetManager()->GetAssets();
	for (uint16 i = 0; i < Assets.size(); ++i)
	{
		TObjectPointer<CAsset> Asset = Assets[i];

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
			
			if (ImGui::BeginPopupContextItem()) // <-- This is using IsItemHovered()
				{
				if (ImGui::MenuItem("Delete"))
				{
					// TODO:
					//DeletePopup(Asset);
				}
				ImGui::EndPopup();
				}
			continue;
		}
		
		ImGui::PushID(i);
		if ((i % NumberPerRow) != 0 && !bFirstInList)
			ImGui::SameLine();

		bFirstInList = false;

		// ImGui::PushID(i+100);
		// ImGui::ButtonEx(std::format("##%s", Asset->FilePath).c_str());

		//if (ImGui::Selectable(names[n], selected == n))
		
		// TODO: Star if unsaved
		if (ImGui::Button(std::format("{}\n\n[{}]", Asset->DisplayName, Asset->ClassName).c_str(), {65,90}))
		{
			Asset->Open();
			// TODO: Determine structure for opening at a per asset basis without having editor code in engine...
			if (TAssetObjectPointer<CMaterialInterface> Material = Asset->Load<CMaterialInterface>())
			{
				GetApplication()->GetApplicationWindow()->GetCanvas<CMaterialEditorPanel>()->SetMaterial(Material);
			}
		}

		if (ImGui::BeginPopupContextItem()) // <-- This is using IsItemHovered()
			{
			static bool dont_ask_me_next_time = true;
			if (ImGui::MenuItem("Save"))
			{
				Asset->Save();
			}
			if (ImGui::MenuItem("Reload"))
			{
				Asset->Reload();
			}
			if (ImGui::MenuItem("Delete"))
			{
				if (dont_ask_me_next_time)
				{
					Asset->Unload();
					Asset->Delete();
				}
				// Later reference check
				else if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
					ImGui::Separator();

					Asset->Load();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						Asset->Unload();
						Asset->Delete();
						ImGui::CloseCurrentPopup();
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0)))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			ImGui::EndPopup();
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
			ImGui::SetDragDropPayload(std::format("ASSET:{}", Asset->ClassName).c_str(), &Asset, sizeof(TSharedPointer<CAsset>));

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
