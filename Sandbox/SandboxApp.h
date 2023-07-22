// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <SkyEngine.h>

#include "Scene/SceneManager.h"
#include "Scenes/Level.h"

class SandboxApp : public SkyEngine::Application
{
public:
	SandboxApp() {}
	~SandboxApp() {}
	
	void ApplicationSetup() override
	{
		Application::ApplicationSetup();
	}
	void Update() override
	{
		Application::Update();

		if (bLoading)
		{
			// std::shared_ptr<EditorScene> NewScene = std::shared_ptr<EditorScene>(new EditorScene("Editor"));			
			// SceneManager::GetInstance()->AddScene(NewScene);
			
			std::shared_ptr<Level> NewScene = std::shared_ptr<Level>(new Level("Level"));
			SceneManager::GetInstance()->AddScene(NewScene);
			
			bLoading = false;
		}
	}	
};

SkyEngine::Application* SkyEngine::CreateApplication()
{
	return new SandboxApp();
}
