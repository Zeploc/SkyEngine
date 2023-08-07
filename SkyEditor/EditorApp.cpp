// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "SEPCH.h"
#include <SkyEngine.h>
#include <Scene/SceneManager.h>
#include <Dependencies/include/glew/glew.h>
#include <Dependencies/include/GLFW/glfw3.h>

#include "Editor/Scene/EditorScene.h"
#include "Editor/Windows/EditorWindowManager.h"

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
		return bSuccessfulSetup;
	}
	
	void Update() override
	{
		Application::Update();

		if (bLoading)
		{			
			bLoading = false;
		}
	}

	void ChangeSize(int w, int h) override
	{
		Application::ChangeSize(w, h);		

		EditorWindowManager::MainWindowSizeChanged(w, h);
	}
};

SkyEngine::Application* SkyEngine::CreateApplication()
{
	return new EditorApplication();
}