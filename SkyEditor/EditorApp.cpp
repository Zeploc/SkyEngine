// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "EditorApp.h"

#include "SEPCH.h"
#include <SkyEngine.h>
#include <Scene/SceneManager.h>

#include "Dependencies/ImGui/imgui.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Editor/EditorLogManager.h"
#include "Editor/EditorViewportLayer.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/UI/ConsoleLog.h"
#include "Editor/UI/EntityPropertiesPanel.h"
#include "Editor/UI/LayerInfoWidget.h"
#include "Editor/Windows/EditorWindowManager.h"
#include "Layers/UILayer.h"
#include "Layers/ViewportLayer.h"


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
	EditorViewportLayer->AddViewportWidget(LayerInfoWidget);
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

void EditorApplication::SetupViewportLayer()
{
	EditorViewportLayer = new CEditorViewportLayer(ApplicationWindow);
	ViewportLayer = EditorViewportLayer;
}

void EditorApplication::Update()
{
	Application::Update();
}

void EditorApplication::Render()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));
	DockSpaceID = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode);
	ImGui::PopStyleColor(1);
	
	ImGuiDockNode* node = ImGui::DockBuilderGetCentralNode(DockSpaceID);
	
	// ImGuiWindowClass centralAlways = {};
	// centralAlways.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_PassthruCentralNode;
	// ImGuiWindowFlags window_flags = 0;
	// window_flags |= ImGuiWindowFlags_NoBackground;
	// window_flags |= ImGuiWindowFlags_NoMouseInputs;
	// window_flags |= ImGuiWindowFlags_NoNavFocus;
	// window_flags |= ImGuiWindowFlags_NoMove;
	// ImGui::SetNextWindowClass(&centralAlways);
	// ImGui::SetNextWindowDockID(node->ID, ImGuiCond_Always);
	// ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	// // ImGui::SetNextWindowBgAlpha(0.0f);
	// ImGui::Begin("Render Viewport", nullptr, window_flags);
	// ImGui::PopStyleVar();
	// // ImGui::Text("Central node: (%f, %f)..(%fx%f)", node->Pos.x, node->Pos.y, node->Size.x, node->Size.y);
	// ImGui::End();
	
	// Draw a red rectangle in the central node just for demonstration purposes
	ImGui::GetBackgroundDrawList()->AddRect
	(
		{node->Pos.x + 4, node->Pos.y + 4},
		{ node->Pos.x + node->Size.x - 4, node->Pos.y + node->Size.y - 4 },
		IM_COL32(255, 255, 255, 200),
		0.5f,
		ImDrawFlags_None,
		2.f
	);
	
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