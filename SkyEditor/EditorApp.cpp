// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <SkyEngine.h>
#include <Scene/SceneManager.h>
#include <Dependencies/include/glew/glew.h>
#include <Dependencies/include/GLFW/glfw3.h>

#include "Editor/Scene/EditorScene.h"
#include "Editor/Windows/EditorWindowManager.h"

class EditorApplication : public SkyEngine::Application
{
public:
	EditorApplication() {}
	~EditorApplication() {}
	
	void ApplicationSetup() override
	{
		Application::ApplicationSetup();

		EditorWindowManager::SetMainWindow(window);
		
		glfwSetWindowFocusCallback(window, EditorWindowManager::FocusChanged);
	}
	void Update() override
	{
		Application::Update();

		if (bLoading)
		{
			std::shared_ptr<EditorScene> NewScene = std::shared_ptr<EditorScene>(new EditorScene("Editor"));			
			SceneManager::GetInstance()->AddScene(NewScene);
			
			// std::shared_ptr<Scene> NewScene = std::shared_ptr<Scene>(new Scene("Empty"));
			// SceneManager::GetInstance()->AddScene(NewScene);
			
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