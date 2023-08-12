// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "EditorApp.h"

#include "SEPCH.h"
#include <SkyEngine.h>
#include <Scene/SceneManager.h>

#include "Dependencies/ImGui/imgui.h"
#include "Editor/EditorLogManager.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/UI/ConsoleLog.h"
#include "Editor/UI/EntityPropertiesPanel.h"
#include "Editor/UI/LayerInfoWidget.h"
#include "Editor/Windows/EditorWindowManager.h"
#include "Layers/UILayer.h"

	
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
	}
	TPointer<CLayerInfoWidget> LayerInfoWidget = std::make_shared<CLayerInfoWidget>();
	UILayer->AddWidget(LayerInfoWidget);
	TPointer<CEntityPropertiesPanel> EntityPropertiesPanel = std::make_shared<CEntityPropertiesPanel>();
	UILayer->AddWidget(EntityPropertiesPanel);
	ConsoleLog = std::make_shared<CConsoleLog>();
	UILayer->AddWidget(ConsoleLog);
	
	// Needed to be in this project to have context global variable
	ImGui::SetCurrentContext(UILayer->GetGuiContext());
	
	return bSuccessfulSetup;
}

void EditorApplication::SetupLogManager()
{
	LogManager = std::make_shared<CEditorLogManager>();
}

void EditorApplication::Update()
{
	Application::Update();
}

void EditorApplication::Render()
{
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