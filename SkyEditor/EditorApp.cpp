// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "EditorApp.h"

#include <fstream>

#include "SEPCH.h"
#include <SkyEngine.h>
#include "Graphics/GL/GLIncludes.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <Scene/SceneManager.h>

#include "Entity/Camera.h"

#include "Dependencies/ImGui/imgui.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "ImGuizmo.h"
#include "Editor/EditorLogManager.h"
#include "Editor/EditorViewportCanvas.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/UI/ConsoleLog.h"
#include "Editor/UI/EntityPropertiesPanel.h"
#include "Editor/UI/LayerInfoWidget.h"
#include "Editor/UI/SceneOutliner.h"
#include "Editor/Windows/EditorWindowManager.h"
#include "Canvas/Canvas.h"
#include "Canvas/ViewportCanvas.h"
#include "Core/StringUtils.h"
#include "Core/Asset/Asset.h"
#include "Editor/Config/EditorSettingsConfig.h"
#include "Editor/UI/ConfigSettingsPanel.h"
#include "Editor/UI/ContentBrowser.h"
#include "Editor/UI/MaterialEditorPanel.h"
#include "Platform/PlatformInterface.h"
#include "Platform/Config/ProjectSettingsConfig.h"
#include "Platform/File/FileManager.h"
#include "Platform/File/PathUtils.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/SceneRenderer.h"
#include "Render/Materials/Material.h"
#include "Render/Meshes/Basic/DefaultMeshes.h"
#include "Render/Meshes/Model/ModelObject.h"
#include "Render/Shaders/PBRShader.h"
#include "Render/Shaders/UndefinedShader.h"
#include "Render/Shaders/UnlitShader.h"
#include "Render/Textures/Texture.h"
#include "Scene/SceneUtils.h"


EditorApplication::EditorApplication() : Application()
{
	EditorApp = this;
	ScenePath = "Levels\\Default.slvl";
}

void EditorApplication::SetProjectDirectory(std::string ExecutablePath)
{
	Application::SetProjectDirectory(ExecutablePath);
}

bool EditorApplication::ApplicationSetup(std::string ExecutablePath)
{
	const bool bSuccessfulSetup = Application::ApplicationSetup(ExecutablePath);
	if (!bSuccessfulSetup)
	{
		return false;
	}
	
	AssetManager.SetAssetPath(ContentPath);
	AssetManager.ScanForAssets();

	TObjectPointer<CAsset> Asset = AssetManager.FindAsset("Textures/Asphalt8_1x1.sasset");
	TAssetObjectPointer<CTexture> TextureObjectPointer = Asset->Load<CTexture>();
	LOG_MESSAGE("Object Pointer valid: {}", TextureObjectPointer.IsValid() ? "True" : "False");
	LOG_MESSAGE("Attempting to unload...");
	Asset->Unload();
	LOG_MESSAGE("Object Pointer valid: {}", TextureObjectPointer.IsValid() ? "True" : "False");

	if (!Asset)
	{
		
	}
	
	// THardPointer<CTexture> BrickTexture = TextureManager.AddNewTexture("Resources\\Images\\StoneWall_2x2.jpg");
	// BrickTexture->Asset->Save();
	// THardPointer<CTexture> BrickTexture = TextureManager.AddNewTexture("Resources\\Images\\Asphalt8_1x1.jpg");
	// BrickTexture->Asset->Save();
	// THardPointer<CTexture> BrickTexture = TextureManager.AddNewTexture("Resources\\Images\\RockyDirt2C_4x4.jpg");
	// BrickTexture->Asset->Save();
	// THardPointer<CTexture> BrickTexture = TextureManager.AddNewTexture("Resources\\Images\\WaterPlain_seamless_S.jpg");
	// BrickTexture->Asset->Save();
	//SetupPlaceholderMaterials();
	
	// TODO: Move to application base default scene
	if (OpenSceneByPath(ContentPath + ScenePath))
	{
		SceneManager::GetInstance()->GetCurrentScene()->SceneName = "Untitled";
	}
	// Failed, open empty scene
	else
	{
		const THardPointer<Scene> NewScene = CreatePointer<Scene>("Untitled");		
		SceneManager::GetInstance()->AddScene(NewScene);
		ViewportCanvas->GetSceneRenderer()->SetSceneTarget(NewScene);
		ViewportCanvas->SetupCamera();
		NewScene->AddEntity(ViewportCanvas->GetViewportCamera());
	}

	// TODO: Swap out with light entity
	Lighting::SetLightPosition({5, 5, 5});
	Lighting::SetSunDirection({3, -1, 5});
	WindowName = "Sky Editor: " + CProjectSettingsConfig::Get()->ProjectName;
	ApplicationWindow->SetWindowTitle(WindowName + " - Untitled");

	THardPointer<CLayerInfoWidget> LayerInfoWidget = std::make_shared<CLayerInfoWidget>();
	EditorViewportLayer->AddViewportWidget(LayerInfoWidget);
	// THardPointer<CEntityPropertiesPanel> EntityPropertiesPanel = std::make_shared<CEntityPropertiesPanel>(ApplicationWindow);
	CEntityPropertiesPanel* EntityPropertiesPanel = new CEntityPropertiesPanel(ApplicationWindow);
	ApplicationWindow->PushLayer(EntityPropertiesPanel);
	// UILayer->AddWidget(EntityPropertiesPanel);
	// THardPointer<CSceneOutliner> SceneOutliner = std::make_shared<CSceneOutliner>(ApplicationWindow);
	CSceneOutliner* SceneOutliner = new CSceneOutliner(ApplicationWindow);
	ApplicationWindow->PushLayer(SceneOutliner);
	// ConsoleLog = std::make_shared<CConsoleLog>();
	ConsoleLog = new CConsoleLog(ApplicationWindow);
	ApplicationWindow->PushLayer(ConsoleLog);
	Cast<CEditorLogManager>(LogManager)->ConsoleLogCreated();
	
	CContentBrowser* ContentBrowser = new CContentBrowser(ApplicationWindow);
	ApplicationWindow->PushLayer(ContentBrowser);
	CMaterialEditorPanel* MaterialEditorPanel = new CMaterialEditorPanel(ApplicationWindow);
	ApplicationWindow->PushLayer(MaterialEditorPanel);
	
	EditorSettingsPanel = new CConfigSettingsPanel(ApplicationWindow, CEditorSettingsConfig::Get());
	ApplicationWindow->PushLayer(EditorSettingsPanel);
	
	// Needed to be in this project to have context global variable
	ImGui::SetCurrentContext(ApplicationWindow->GetCanvasManager().GetGuiContext());
	
	return bSuccessfulSetup;
}

void EditorApplication::SetupPlaceholderMaterials()
{
	TAssetObjectPointer<CTexture> BrickTexture = GetTextureManager()->FindTexture("Resources/Images/StoneWall_2x2.jpg");
	
	TAssetObjectPointer<CMaterial_PBR> BrickMaterial;
	
	TObjectPointer<CAsset> StoneWallMaterialAsset = AssetManager.FindAsset("Materials/StoneWallMaterial.sasset");
	// if (StoneWallMaterialAsset)
	// {
	// 	BrickMaterial = StoneWallMaterialAsset->Load<CMaterial_PBR>();
	// }
	// else
	BrickMaterial = std::make_shared<CMaterial_PBR>("BrickMaterial");
	// THardPointer<TMaterial<CUndefinedShader>> BrickMaterial = std::make_shared<TMaterial<CUndefinedShader>>("BrickMaterial", ShaderManager::GetUndefinedShader("BaseProgram"));
	BrickMaterial->Params.DiffuseTexture = BrickTexture;
	GetMaterialManager()->AddMaterial(BrickMaterial);

	// Would be nice to be able to copy an existing material as a template
	THardPointer<CMaterial_PBR> ColouredBrickMaterial = std::make_shared<CMaterial_PBR>("ColouredBrickMaterial");
	ColouredBrickMaterial->Params.DiffuseTexture = BrickTexture;
	ColouredBrickMaterial->Params.DiffuseColour = {0.5f, 0.3f, 0.3f, 1.0f};
	GetMaterialManager()->AddMaterial(ColouredBrickMaterial);
	
	THardPointer<CMaterial_PBR> ColouredBrickPlaneMaterial = std::make_shared<CMaterial_PBR>("ColouredBrickPlaneMaterial");
	ColouredBrickPlaneMaterial->Params.DiffuseTexture = BrickTexture;
	ColouredBrickPlaneMaterial->Params.DiffuseColour = {0.5f, 0.3f, 0.3f, 1.0f};
	ColouredBrickPlaneMaterial->bTwoSided = true;
	GetMaterialManager()->AddMaterial(ColouredBrickPlaneMaterial);
	
	TAssetObjectPointer<CMaterial_PBR> CliffMaterial = std::make_shared<CMaterial_PBR>("CliffMaterial");
	TAssetObjectPointer<CTexture> CliffTexture = GetTextureManager()->FindTexture("Resources/Images/SmoothCliff_1024.jpg");
	CliffMaterial->Params.DiffuseTexture = BrickTexture;
	CliffMaterial->Params.DiffuseColour = {0.1f, 0.8f, 0.3f, 1.0f};	
	GetMaterialManager()->AddMaterial(CliffMaterial);

	TAssetObjectPointer<CMaterial_PBR> PlaneMaterial = std::make_shared<CMaterial_PBR>("PlaneMaterial");
	PlaneMaterial->Params.DiffuseColour = {0.5f, 0.5f, 0.5f, 1.0f};
	PlaneMaterial->bTwoSided = true;
	GetMaterialManager()->AddMaterial(PlaneMaterial);
	
	TAssetObjectPointer<CMaterial_Unlit> BoxMaterial = std::make_shared<CMaterial_Unlit>("BoxMaterial");
	BoxMaterial->Params.DiffuseColour = SVector4(1.0f, 0.1f, 0.1f, 1.0f);
	GetMaterialManager()->AddMaterial(BoxMaterial);	
	
	TObjectPointer<CAsset> MaterialAsset;
	TArray<TAssetObjectPointer<CMaterialInterface>> Materials = {BrickMaterial, ColouredBrickMaterial, ColouredBrickPlaneMaterial, CliffMaterial, PlaneMaterial, BoxMaterial};
	for (TAssetObjectPointer<CMaterialInterface> Material : Materials)
	{
		MaterialAsset = AssetManager.AddAsset(std::string("Materials/") + Material->GetMaterialName() + std::string(".sasset"), Material->GetAssetClassName());
		MaterialAsset->SetDefaultObject(Material);
		MaterialAsset->Save();
	}
	
}

void EditorApplication::SetupConfigs()
{
	Application::SetupConfigs();
	
	CConfig::RegisterConfig<CEditorSettingsConfig>();
	const THardPointer<CProjectSettingsConfig> ProjectSettings = CProjectSettingsConfig::Get();
	const THardPointer<CEditorSettingsConfig> EditorSettings = CEditorSettingsConfig::Get();
	LogManager->DisplayMessage("Resolution: " + ProjectSettings->DefaultResolution.ToString()); 
	LogManager->DisplayMessage("Graphics Mode: " + std::to_string(ProjectSettings->GraphicsMode));
	MainWindowSize = TVector2<int>(EditorSettings->Resolution);
	GraphicsApiType = (EGraphicsAPI)EditorSettings->GraphicsMode;
}

void EditorApplication::SetupLogManager()
{
	LogManager = std::make_shared<CEditorLogManager>();
}

void EditorApplication::SetupViewportLayer()
{
	EditorViewportLayer = new CEditorViewportCanvas(ApplicationWindow);
	ViewportCanvas = EditorViewportLayer;
}

void EditorApplication::Update()
{
	Application::Update();
}

void EditorApplication::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Scene", "CTRL+O"))
			{
				OpenNewScene();
			}
			if (ImGui::MenuItem("Save Scene", "CTRL+S"))
			{
				SaveScene();
			}
			if (ImGui::MenuItem("Save Scene As", "CTRL+SHIFT+S"))
			{
				SaveScene(true);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Editor Settings", ""))
			{
				EditorSettingsPanel->Open();
				ImGui::FocusWindow(ImGui::FindWindowByName(EditorSettingsPanel->GetName().c_str()));
			}
			if (ImGui::MenuItem("Project Settings", ""))
			{
				if (!ProjectSettingsPanel)
				{
					ProjectSettingsPanel = new CConfigSettingsPanel(ApplicationWindow, CProjectSettingsConfig::Get());
					ApplicationWindow->PushLayer(ProjectSettingsPanel);
				}
				else
				{
					ProjectSettingsPanel->Open();
					ImGui::FocusWindow(ImGui::FindWindowByName(ProjectSettingsPanel->GetName().c_str()));
				}	
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Entity"))
		{
			if (ImGui::BeginMenu("Add New Entity"))
			{
				if (ImGui::MenuItem("Empty"))
				{
					EditorViewportLayer->CreateEntity();
				}
				else if (ImGui::MenuItem("Cube"))
				{
					EditorViewportLayer->CreateEntity(DefaultMesh::GetCube());
				}
				else if (ImGui::MenuItem("Sphere"))
				{
					EditorViewportLayer->CreateEntity(DefaultMesh::GetSphere());
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Delete Entity", "DEL"))
			{
				EditorViewportLayer->DeleteSelected();
			}
			ImGui::Separator();
			THardPointer<Entity> SelectedEntity = EditorViewportLayer->GetSelectedEntity();
			if (ImGui::BeginMenu("Add New Component", SelectedEntity != nullptr))
			{
				THardPointer<CMeshComponent> NewMeshComponent = nullptr;
				// if (ImGui::MenuItem("Mesh Component"))
				// {
				// 	NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, 50.0f, 50.0f, 50.0f, nullptr);
				// }
				if (ImGui::MenuItem("Cube Component"))
				{
					NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, DefaultMesh::GetCube(), nullptr);
				}
				if (ImGui::MenuItem("Plane Component"))
				{
					NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, DefaultMesh::GetPlane(), nullptr);
				}
				if (ImGui::MenuItem("Pyramid Component"))
				{
					NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, DefaultMesh::GetPyramid(), nullptr);
				}
				if (ImGui::MenuItem("Sphere Component"))
				{
					NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, DefaultMesh::GetSphere(), nullptr);
				}
				if (NewMeshComponent)
				{
					SelectedEntity->AddComponent(NewMeshComponent);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void EditorApplication::Render()
{
	DockSpaceID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode);

	MainMenuBar();
	
	Application::Render();	

	EditorWindowManager::RenderWindows();	
}

void EditorApplication::OnEvent(CEvent& Event)
{
	Application::OnEvent(Event);
	
	EventDispatcher dispatcher(Event);
	dispatcher.Dispatch<CKeyPressedEvent>(SE_BIND_EVENT_FN(EditorApplication::OnKeyPressedEvent));
}

bool EditorApplication::OnKeyPressedEvent(CKeyPressedEvent& Event)
{
	if (Event.GetMods() & CWindowInput::ModiferType::Control)
	{
		if (Event.GetKeyCode() == GLFW_KEY_O)
		{
			OpenNewScene();
			return true;
		}
		if (Event.GetKeyCode() == GLFW_KEY_S)
		{
			if (Event.GetMods() & CWindowInput::ModiferType::Shift)
			{
				SaveScene(true);
			}
			else
			{
				SaveScene();
			}
			return true;
		}
	}
	return false;
}

void EditorApplication::OnExit()
{
	EditorWindowManager::CleanupWindows();
	Application::OnExit();
}

void EditorApplication::OpenNewScene()
{
	std::string FilePath;
	if (!CFileManager::OpenFile(FilePath, "slvl", ContentPath))
	{
		// No file opened (cancelled)
		return;
	}
	OpenSceneByPath(FilePath);
}

bool EditorApplication::UnsavedSceneCheck()
{
	TAssetObjectPointer<Scene> CurrentScene = SceneManager::GetInstance()->GetCurrentScene();
	if (CurrentScene)
	{
		std::stringstream SceneStream;
		CurrentScene->Serialize(SceneStream);
		std::string _;
		std::string AssetData;
		std::getline(SceneStream, _, '[');
		std::getline(SceneStream, AssetData, ']');

		std::string ExistingFile;
		if (CFileManager::ReadFile(PathUtils::CombinePath(ContentPath, CurrentScene->Asset->FilePath), ExistingFile))
		{
			const std::string Basic_String = SceneStream.str();
			const std::string FilePrefix = std::format("[{}]\n", Scene::GetStaticName());
			StringUtils::ReplaceFirst(ExistingFile, FilePrefix, "");
			if (ExistingFile != Basic_String)
			{
				const int UnsavedChangesResponse = PlatformInterface->DisplayMessageBox("Unsaved Changes!", "Unsaved changes, do you want to discard and continue?", MB_OKCANCEL);
				if (UnsavedChangesResponse == IDCANCEL)
				{
					return false;
				}
			}		
		}
	}
	return true;
}

bool EditorApplication::OpenSceneByPath(std::string FilePath)
{
	StringUtils::Replace(FilePath, ContentPath, "");
	TObjectPointer<CAsset> NewSceneAsset = AssetManager.FindAsset(FilePath);
	TAssetObjectPointer<Scene> NewScene = NewSceneAsset->Load<Scene>();	
	if (!NewScene)
	{
		PlatformInterface->DisplayMessageBox("Failed to load scene", FilePath, MB_OK);
		NewScene = nullptr;
		return false;
	}
	// NewScene->Open();
	if (!OpenScene(NewScene))
	{
		return false;	
	}
	return true;
}

bool EditorApplication::OpenScene(TAssetObjectPointer<Scene> NewScene)
{	
	if (!UnsavedSceneCheck())
	{
		return false;
	}
	ApplicationWindow->SetWindowTitle(WindowName + " - " + NewScene->SceneName);
	const bool bOpenSuccessful = Application::OpenScene(NewScene);
	if (bOpenSuccessful)
	{
		ScenePath = NewScene->Asset->FilePath;		
	}
	return bOpenSuccessful;
}

void EditorApplication::SaveScene(bool bAsNew)
{
	const TAssetObjectPointer<Scene> Scene = SceneManager::GetInstance()->GetCurrentScene();
	bAsNew |= Scene->SceneName == "Untitled";
	if (bAsNew)
	{
		const std::string FileName = PathUtils::GetFileName(ScenePath);
		std::string Directory = PathUtils::GetDirectory(ScenePath);
		Directory = ContentPath + Directory;
		std::string ChosenFilePath;
		if (!CFileManager::SaveAsFile(ChosenFilePath, "Level (*.slvl)\0*.slvl\0Text\0*.TXT\0", FileName, Directory))
		{
			return;
		}
		PathUtils::SetExtension(ChosenFilePath, "slvl");
		ScenePath = ChosenFilePath;
		StringUtils::Replace(ScenePath, ContentPath, "");
		TObjectPointer<CAsset> NewSceneAsset = AssetManager.AddAsset(ScenePath, Scene::GetStaticName());
		Scene->Asset->DisconnectObject();
		NewSceneAsset->SetDefaultObject(Scene);		
	}
	const std::string SceneName = PathUtils::GetFileName(ScenePath, false);
	Scene->SceneName = SceneName;
	ApplicationWindow->SetWindowTitle(WindowName + " - \"" + SceneName + "\"");
	if (Scene->Asset->Save())
	{
		GetPlatformInterface()->DisplayMessageBox("Saved scene", std::format("Saved Scene \"{}\"!", SceneName), MB_OK);
	}
}

SkyEngine::Application* SkyEngine::CreateApplication()
{
	return new EditorApplication();
}
