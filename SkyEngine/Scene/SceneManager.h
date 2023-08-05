// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //

class Scene;

class ENGINE_API SceneManager
{
public:
	std::map<std::string, TPointer<Scene>> Scenes;

	void AddScene(TPointer<Scene> _Scene);

	void RemoveScene(std::string SceneName);

	void RemoveScene(TPointer<Scene> _Scene);

	void SwitchScene(std::string SceneName, bool _bInstant = false);

	void UpdateCurrentScene();

	void RenderCurrentScene();

	TPointer<Scene> GetCurrentScene();

	std::string CurrentScene = "";
	std::string SceneToSwitch = "";

	// Singleton
public:
	static TPointer<SceneManager> GetInstance();

	static void DestoryInstance();

	~SceneManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static TPointer<SceneManager> m_pSceneManager;

	SceneManager();

	SceneManager(const SceneManager&); // Don't Implement
	void operator=(const SceneManager&); // Don't implement
};
