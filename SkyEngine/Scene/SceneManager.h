// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class IRenderer;
class Scene;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API SceneManager
{
public:

	void AddScene(TPointer<Scene> _Scene);

	void RemoveScene(std::string SceneName);

	void RemoveScene(TPointer<Scene> _Scene);

	void SwitchScene(std::string SceneName, bool _bInstant = false);

	void UpdateCurrentScene();

	void RenderCurrentScene();

	TPointer<Scene> GetCurrentScene();

	// Singleton
public:
	static TPointer<SceneManager> GetInstance();

	static void DestoryInstance();

	~SceneManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	std::map<std::string, TPointer<Scene>> Scenes;
	static TPointer<SceneManager> m_pSceneManager;
	std::string CurrentScene = "";
	std::string SceneToSwitch = "";

	SceneManager();

	SceneManager(const SceneManager&); // Don't Implement
	void operator=(const SceneManager&); // Don't implement
};
