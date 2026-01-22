// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"

class Scene;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API SceneManager
{
public:

	void AddScene(const TAssetObjectPointer<Scene>& InScene);

	void RemoveScene(std::string SceneName);

	void RemoveScene(TAssetObjectPointer<Scene> InScene);

	void SwitchScene(std::string SceneName, bool _bInstant = false);

	void UpdateCurrentScene();

	void RenderCurrentScene();

	TAssetObjectPointer<Scene> GetCurrentScene();

	// Singleton
public:
	static TSharedPointer<SceneManager> GetInstance();

	static void DestoryInstance();

	~SceneManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	std::map<std::string, TAssetObjectPointer<Scene>> Scenes;
	static TSharedPointer<SceneManager> m_pSceneManager;
	std::string CurrentScene = "";
	std::string SceneToSwitch = "";

	SceneManager();

	SceneManager(const SceneManager&); // Don't Implement
	void operator=(const SceneManager&); // Don't implement
};
