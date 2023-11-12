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
#include "Scene/SceneUtils.h"


EditorApplication::EditorApplication() : Application()
{
	EditorApp = this;
	SceneFilePath = "Resources\\Levels\\Untitled.slvl";
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
	// TODO: Move to application base default scene
	if (bSuccessfulSetup)
	{			
		TPointer<EditorScene> NewScene = TPointer<EditorScene>(new EditorScene("Untitled"));		
		SceneManager::GetInstance()->AddScene(NewScene);
		ViewportCanvas->GetSceneRenderer()->SetSceneTarget(NewScene);
		ViewportCanvas->SetupCamera();
		NewScene->AddEntity(ViewportCanvas->GetViewportCamera());
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
	std::string FileContents;
	if (!CFileManager::ReadFile(FilePath, FileContents))
	{
		PlatformInterface->DisplayMessageBox("Failed to read file", FilePath, MB_OK);
		return;
	}
	std::stringstream FileStream(FileContents);

	const TPointer<Scene> NewScene = CreatePointer<Scene>("LoadedScene");
	if (!NewScene->Deserialize(FileStream))
	{
		PlatformInterface->DisplayMessageBox("Failed to deserialize scene", FilePath, MB_OK);		
		return;
	}
	ApplicationWindow->SetWindowTitle("Sky Editor: " + NewScene->SceneName);
	TPointer<Scene> PreviousScene = SceneManager::GetInstance()->GetCurrentScene();
	SceneManager::GetInstance()->AddScene(NewScene);
	SceneManager::GetInstance()->SwitchScene(NewScene->SceneName, false);
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
	// SceneManager::GetInstance()->RemoveScene(PreviousScene);
}

void EditorApplication::SaveScene(bool bAsNew)
{
	const TPointer<Scene> Scene = SceneManager::GetInstance()->GetCurrentScene();
	bAsNew |= Scene->SceneName == "Untitled";
	if (bAsNew)
	{
		const std::string FileName = PathUtils::GetFileName(SceneFilePath);
		std::string Directory = PathUtils::GetDirectory(SceneFilePath);
		Directory = ContentPath + Directory;
		std::string ChosenFilePath;
		if (!CFileManager::SaveAsFile(ChosenFilePath, "Level (*.slvl)\0*.slvl\0Text\0*.TXT\0", FileName, Directory))
		{
			return;
		}
		PathUtils::SetExtension(ChosenFilePath, "slvl");
		SceneFilePath = ChosenFilePath;
	}
	const std::string SceneName = PathUtils::GetFileName(SceneFilePath, false);
	Scene->SceneName = SceneName;
	ApplicationWindow->SetWindowTitle("Sky Editor: \"" + SceneName + "\"");
	const std::string SerializedScene = Scene->Serialize();
	CFileManager::SaveFile(SceneFilePath, SerializedScene);
	GetPlatformInterface()->DisplayMessageBox("Saved scene", std::format("Saved Scene \"{}\"!", SceneName), MB_OK);
}

SkyEngine::Application* SkyEngine::CreateApplication()
{
	return new EditorApplication();
}
