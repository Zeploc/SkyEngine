// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "SEPCH.h"
#include <SkyEngine.h>
#include <Scene/SceneManager.h>
#include <Dependencies/include/glew/glew.h>
#include <Dependencies/include/GLFW/glfw3.h>

#include "Dependencies/ImGui/imgui.h"
#include "Editor/Scene/EditorScene.h"
#include "Editor/UI/LayerInfoWidget.h"
#include "Editor/Windows/EditorWindowManager.h"
#include "Layers/UILayer.h"

class EditorApplication : public SkyEngine::Application
{
public:	
	EditorApplication() : Application() {}
	~EditorApplication() {}

	bool ApplicationSetup() override
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
		
		// Needed to be in this project to have context global variable
		ImGui::SetCurrentContext(UILayer->GetGuiContext());
		
		return bSuccessfulSetup;
	}
	
	void Update() override
	{
		Application::Update();
	}

	void Render() override
	{
		Application::Render();

		EditorWindowManager::RenderWindows();	
	}

	void ChangeSize(int w, int h) override
	{
		Application::ChangeSize(w, h);		

		EditorWindowManager::MainWindowSizeChanged(w, h);
	}
	void OnExit() override
	{
		EditorWindowManager::CleanupWindows();
		Application::OnExit();
	}
};

SkyEngine::Application* SkyEngine::CreateApplication()
{
	return new EditorApplication();
}