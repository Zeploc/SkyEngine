// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "EditorApp.h"

#include <fstream>

#include "SEPCH.h"
#include <SkyEngine.h>
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
#include "Platform/PlatformInterface.h"
#include "Platform/File/FileManager.h"
#include "Platform/File/PathUtils.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/SceneRenderer.h"
#include "Render/Materials/Material.h"
#include "Render/Shaders/PBRShader.h"
#include "Render/Shaders/UndefinedShader.h"
#include "Render/Shaders/UnlitShader.h"
#include "Scene/SceneUtils.h"


EditorApplication::EditorApplication() : Application()
{
	EditorApp = this;
	ScenePath = "Resources\\Levels\\Default.slvl";
}

void EditorApplication::SetContentDirectory(std::string ExecutablePath)
{
	uint64_t DirectoryEndIndex = ExecutablePath.find_last_of("\\");
	ContentPath = ExecutablePath.substr(0, DirectoryEndIndex);
	DirectoryEndIndex = ContentPath.find_last_of("\\");
	ContentPath = ContentPath.substr(0, DirectoryEndIndex);
	DirectoryEndIndex = ContentPath.find_last_of("\\");
	ContentPath = ContentPath.substr(0, DirectoryEndIndex);
	DirectoryEndIndex = ContentPath.find_last_of("\\");
	ContentPath = ContentPath.substr(0, DirectoryEndIndex);

	// TODO: Should be moved out of editor
	ContentPath += "\\SkyEditor\\";
}

bool EditorApplication::ApplicationSetup(std::string ExecutablePath)
{
	const bool bSuccessfulSetup = Application::ApplicationSetup(ExecutablePath);
	
	SetContentDirectory(ExecutablePath);
	SetupPlaceholderMaterials();
	
	// TODO: Move to application base default scene
	if (bSuccessfulSetup)
	{
		if (OpenScene(ContentPath + ScenePath))
		{
			SceneManager::GetInstance()->GetCurrentScene()->SceneName = "Untitled";
		}
		// Failed, open empty scene
		else
		{
			const TPointer<Scene> NewScene = CreatePointer<Scene>("Untitled");		
			SceneManager::GetInstance()->AddScene(NewScene);
			ViewportCanvas->GetSceneRenderer()->SetSceneTarget(NewScene);
			ViewportCanvas->SetupCamera();
			NewScene->AddEntity(ViewportCanvas->GetViewportCamera());
		}

		// TODO: Swap out with light entity
		Lighting::SetLightPosition({5, 5, 5});
		Lighting::SetSunDirection({3, -1, 5});
		ApplicationWindow->SetWindowTitle("Sky Editor: Untitled");
	}
	TPointer<CLayerInfoWidget> LayerInfoWidget = std::make_shared<CLayerInfoWidget>();
	EditorViewportLayer->AddViewportWidget(LayerInfoWidget);
	// TPointer<CEntityPropertiesPanel> EntityPropertiesPanel = std::make_shared<CEntityPropertiesPanel>(ApplicationWindow);
	CEntityPropertiesPanel* EntityPropertiesPanel = new CEntityPropertiesPanel(ApplicationWindow);
	ApplicationWindow->PushLayer(EntityPropertiesPanel);
	// UILayer->AddWidget(EntityPropertiesPanel);
	// TPointer<CSceneOutliner> SceneOutliner = std::make_shared<CSceneOutliner>(ApplicationWindow);
	CSceneOutliner* SceneOutliner = new CSceneOutliner(ApplicationWindow);
	ApplicationWindow->PushLayer(SceneOutliner);
	// ConsoleLog = std::make_shared<CConsoleLog>();
	ConsoleLog = new CConsoleLog(ApplicationWindow);
	ApplicationWindow->PushLayer(ConsoleLog);
	
	// Needed to be in this project to have context global variable
	ImGui::SetCurrentContext(ApplicationWindow->GetCanvasManager().GetGuiContext());
	
	return bSuccessfulSetup;
}

void EditorApplication::SetupPlaceholderMaterials()
{
	TPointer<CTexture> BrickTexture = GetGraphicsAPI()->GetTexture("Resources/Images/StoneWall_2x2.jpg");
	// TPointer<CMaterial_PBR> BrickMaterial = std::make_shared<CMaterial_PBR>();
	TPointer<TMaterial<CUndefinedShader>> BrickMaterial = std::make_shared<TMaterial<CUndefinedShader>>("BrickMaterial", ShaderManager::GetUndefinedShader("BaseProgram"));
	BrickMaterial->Params.DiffuseTexture = BrickTexture;
	GetMaterialManager()->AddMaterial(BrickMaterial);
	
	// Would be nice to be able to copy an existing material as a template
	TPointer<CMaterial_PBR> ColouredBrickMaterial = std::make_shared<CMaterial_PBR>("ColouredBrickMaterial");
	ColouredBrickMaterial->Params.DiffuseTexture = BrickTexture;
	ColouredBrickMaterial->Params.DiffuseColour = {0.5f, 0.3f, 0.3f, 1.0f};
	GetMaterialManager()->AddMaterial(ColouredBrickMaterial);
	
	TPointer<CMaterial_PBR> ColouredBrickPlaneMaterial = std::make_shared<CMaterial_PBR>("ColouredBrickPlaneMaterial");
	ColouredBrickPlaneMaterial->Params.DiffuseTexture = BrickTexture;
	ColouredBrickPlaneMaterial->Params.DiffuseColour = {0.5f, 0.3f, 0.3f, 1.0f};
	ColouredBrickPlaneMaterial->bTwoSided = true;
	GetMaterialManager()->AddMaterial(ColouredBrickPlaneMaterial);
	
	TPointer<CMaterial_PBR> CliffMaterial = std::make_shared<CMaterial_PBR>("CliffMaterial");
	TPointer<CTexture> CliffTexture = GetGraphicsAPI()->GetTexture("Resources/Images/SmoothCliff_1024.jpg");
	CliffMaterial->Params.DiffuseTexture = BrickTexture;
	CliffMaterial->Params.DiffuseColour = {0.1f, 0.8f, 0.3f, 1.0f};	
	GetMaterialManager()->AddMaterial(CliffMaterial);

	TPointer<CMaterial_PBR> PlaneMaterial = std::make_shared<CMaterial_PBR>("PlaneMaterial");
	PlaneMaterial->Params.DiffuseColour = {0.5f, 0.5f, 0.5f, 1.0f};
	PlaneMaterial->bTwoSided = true;
	GetMaterialManager()->AddMaterial(PlaneMaterial);
	
	TPointer<CMaterial_Unlit> BoxMaterial = std::make_shared<CMaterial_Unlit>("BoxMaterial");
	BoxMaterial->Params.DiffuseColour = SVector4(1.0f, 0.1f, 0.1f, 1.0f);
	GetMaterialManager()->AddMaterial(BoxMaterial);
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
				OpenScene();
			}
			if (ImGui::MenuItem("Save Scene", "CTRL+S"))
			{
				SaveScene();
			}
			if (ImGui::MenuItem("Save Scene As", "CTRL+SHIFT+S"))
			{
				SaveScene(true);
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
					const TPointer<Entity> NewEntity(new Entity(STransform()));
					SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewEntity);
					EditorViewportLayer->SelectEntity(NewEntity, true);
				}
				else if (ImGui::MenuItem("Cube"))
				{
					const TPointer<Entity> NewEntity(new Entity(STransform(), "Cube"));
					SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewEntity);
					EditorViewportLayer->SelectEntity(NewEntity, true);
					TPointer<CMeshComponent> NewMeshComponent = std::make_shared<CMeshComponent>(NewEntity, MESH_CUBE, nullptr);
					NewEntity->AddComponent(NewMeshComponent);
				}
				else if (ImGui::MenuItem("Sphere"))
				{
					const TPointer<Entity> NewEntity(new Entity(STransform(), "Sphere"));
					SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewEntity);
					EditorViewportLayer->SelectEntity(NewEntity, true);
					TPointer<CMeshComponent> NewMeshComponent = std::make_shared<CMeshComponent>(NewEntity, MESH_SPHERE, nullptr);
					NewEntity->AddComponent(NewMeshComponent);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Delete Entity", "DEL"))
			{
				EditorViewportLayer->DeleteSelected();
			}
			ImGui::Separator();
			TPointer<Entity> SelectedEntity = EditorViewportLayer->GetSelectedEntity();
			if (ImGui::BeginMenu("Add New Component", SelectedEntity != nullptr))
			{
				TPointer<CMeshComponent> NewMeshComponent = nullptr;
				// if (ImGui::MenuItem("Mesh Component"))
				// {
				// 	NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, 50.0f, 50.0f, 50.0f, nullptr);
				// }
				if (ImGui::MenuItem("Cube Component"))
				{
					NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, MESH_CUBE, nullptr);
				}
				if (ImGui::MenuItem("Plane Component"))
				{
					NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, MESH_PLANE, nullptr);
				}
				if (ImGui::MenuItem("Pyramid Component"))
				{
					NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, MESH_PYRAMID, nullptr);
				}
				if (ImGui::MenuItem("Sphere Component"))
				{
					NewMeshComponent = std::make_shared<CMeshComponent>(SelectedEntity, MESH_SPHERE, nullptr);
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

void EditorApplication::ChangeSize(int w, int h)
{
	Application::ChangeSize(w, h);		

	EditorWindowManager::MainWindowSizeChanged(w, h);
}
void EditorApplication::OnExit()
{
	EditorWindowManager::CleanupWindows();
	Application::OnExit();
}

void EditorApplication::OpenScene()
{
	std::string FilePath;
	if (!CFileManager::OpenFile(FilePath))
	{
		// No file opened (cancelled)
		return;
	}
	OpenScene(FilePath);
}

bool EditorApplication::OpenScene(std::string FilePath)
{
	std::string FileContents;
	if (!CFileManager::ReadFile(FilePath, FileContents))
	{
		PlatformInterface->DisplayMessageBox("Failed to read file", FilePath, MB_OK);
		return false;
	}
	std::stringstream FileStream(FileContents);

	TPointer<Scene> NewScene = CreatePointer<Scene>("LoadedScene");
	if (!NewScene->Deserialize(FileStream))
	{
		PlatformInterface->DisplayMessageBox("Failed to deserialize scene", FilePath, MB_OK);
		NewScene = nullptr;
		return false;
	}
	ApplicationWindow->SetWindowTitle("Sky Editor: " + NewScene->SceneName);
	TPointer<Scene> PreviousScene = SceneManager::GetInstance()->GetCurrentScene();
	SceneManager::GetInstance()->AddScene(NewScene);
	SceneManager::GetInstance()->SwitchScene(NewScene->SceneName, true);
	const TPointer<Camera> FoundCamera = SceneUtils::FindEntityOfClass<Camera>();
	ViewportCanvas->GetSceneRenderer()->SetSceneTarget(NewScene);
	if (FoundCamera)
	{
		ViewportCanvas->SetCamera(FoundCamera);
	}
	else
	{
		ViewportCanvas->SetupCamera();
		NewScene->AddEntity(ViewportCanvas->GetViewportCamera());
	}
	SceneManager::GetInstance()->RemoveScene(PreviousScene);
	return true;
}

void EditorApplication::SaveScene(bool bAsNew)
{
	const TPointer<Scene> Scene = SceneManager::GetInstance()->GetCurrentScene();
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
	}
	const std::string SceneName = PathUtils::GetFileName(ScenePath, false);
	Scene->SceneName = SceneName;
	ApplicationWindow->SetWindowTitle("Sky Editor: \"" + SceneName + "\"");
	const std::string SerializedScene = Scene->Serialize();
	CFileManager::SaveFile(ScenePath, SerializedScene);
	GetPlatformInterface()->DisplayMessageBox("Saved scene", std::format("Saved Scene \"{}\"!", SceneName), MB_OK);
}

SkyEngine::Application* SkyEngine::CreateApplication()
{
	return new EditorApplication();
}
