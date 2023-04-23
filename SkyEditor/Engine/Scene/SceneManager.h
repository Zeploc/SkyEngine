// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <map>
#include <memory>
#include <string>

class Scene;

class SceneManager
{
public:
	std::map<std::string, std::shared_ptr<Scene>> Scenes;

	void AddScene(std::shared_ptr<Scene> _Scene);

	void RemoveScene(std::string SceneName);

	void RemoveScene(std::shared_ptr<Scene> _Scene);

	void SwitchScene(std::string SceneName, bool _bInstant = false);

	void UpdateCurrentScene();

	void RenderCurrentScene();

	std::shared_ptr<Scene> GetCurrentScene();

	std::string CurrentScene = "";
	std::string SceneToSwitch = "";

	// Singleton
public:
	static std::shared_ptr<SceneManager> GetInstance();

	static void DestoryInstance();

	~SceneManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static std::shared_ptr<SceneManager> m_pSceneManager;

	SceneManager();

	SceneManager(const SceneManager&); // Don't Implement
	void operator=(const SceneManager&); // Don't implement
};
