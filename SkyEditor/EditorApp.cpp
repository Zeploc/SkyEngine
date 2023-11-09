// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "EditorApp.h"

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
#include "Platform/Window/EngineWindow.h"
#include "Render/SceneRenderer.h"


EditorApplication::EditorApplication() : Application()
{
	EditorApp = this;
}

bool EditorApplication::ApplicationSetup()
{
	const bool bSuccessfulSetup = Application::ApplicationSetup();
	// TODO: Move to application base default scene
	if (bSuccessfulSetup)
	{			
		TPointer<EditorScene> NewScene = TPointer<EditorScene>(new EditorScene("Editor"));			
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
			if (ImGui::MenuItem("Open Scene", "CTRL+O")) {}
			if (ImGui::MenuItem("Save Scene", "CTRL+S")) {}
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
			if (ImGui::MenuItem("Add New Entity"))
			{
				const TPointer<Entity> NewEntity(new Entity(STransform()));
				SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewEntity);
				EditorViewportLayer->SelectEntity(NewEntity, true);
			}
			if (ImGui::MenuItem("Delete Entity", "DEL")) {}
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

SkyEngine::Application* SkyEngine::CreateApplication()
{
	return new EditorApplication();
}