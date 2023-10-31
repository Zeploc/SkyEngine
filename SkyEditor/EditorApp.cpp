// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "EditorApp.h"

#include "SEPCH.h"
#include <SkyEngine.h>
#include <Scene/SceneManager.h>

#include "Dependencies/ImGui/imgui.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Editor/EditorLogManager.h"
#include "Editor/EditorViewportCanvas.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/UI/ConsoleLog.h"
#include "Editor/UI/EntityPropertiesPanel.h"
#include "Editor/UI/LayerInfoWidget.h"
#include "Editor/UI/SceneOutliner.h"
#include "Editor/Windows/EditorWindowManager.h"
#include "Canvas/Canvas.h"
#include "Canvas/UICanvas.h"
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
		ViewportLayer->GetSceneRenderer()->SetSceneTarget(NewScene);
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
	ViewportLayer = EditorViewportLayer;
}

void EditorApplication::Update()
{
	Application::Update();
}

void EditorApplication::Render()
{
	DockSpaceID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode);
	
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