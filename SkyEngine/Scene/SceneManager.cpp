// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "SceneManager.h"

// Library Includes //


// Engine Includes //
#include "Scene.h"
#include "Core/Asset/Asset.h"
#include "System/LogManager.h"

// Local Includes //

// Static Variables //
THardPointer<SceneManager> SceneManager::m_pSceneManager;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
SceneManager::SceneManager()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
SceneManager::~SceneManager()
{
	for (const auto&[name, scene]: Scenes)
	{
		scene->DeleteScene();
	}
	Scenes.clear();
}

/************************************************************
#--Description--#: 	Adds scene to scene manager
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Scene to add
#--Return--#: 		NA
************************************************************/
void SceneManager::AddScene(const TAssetObjectPointer<Scene>& InScene)
{
	if (Scenes.contains(InScene->SceneName))
	{
		ensure(false, "Attempted to add scene with same name as existing!");
		return;
	}
	if (Scenes.empty())
	{
		Scenes[InScene->SceneName] = InScene;

		SceneToSwitch = InScene->SceneName;
		SwitchScene(SceneToSwitch, true);
	}
	else
	{
		Scenes[InScene->SceneName] = InScene;
	}
}

/************************************************************
#--Description--#: 	Removes scene from scene manager
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Name of scene to remove
#--Return--#: 		NA
************************************************************/
void SceneManager::RemoveScene(std::string SceneName)
{
	if (!Scenes.contains(SceneName))
	{
		// Scene Doesn't exist
		CLogManager::Get()->DisplayMessage("Could not find scene " + SceneName);
		return;
	}
	Scenes[SceneName]->DeleteScene();
	Scenes.erase(SceneName);
}

/************************************************************
#--Description--#: 	Removes scene from scene manager
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Scene to remove
#--Return--#: 		NA
************************************************************/
void SceneManager::RemoveScene(TAssetObjectPointer<Scene> InScene)
{
	for (auto it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		if ((*it).second == InScene)
		{
			(*it).second->DeleteScene();
			Scenes.erase(it);
			return;
		}
	}
}

/************************************************************
#--Description--#: 	Switches to scene with matching name
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in scene name to switch to
#--Return--#: 		Returns true if scene found, if not returns false
************************************************************/
void SceneManager::SwitchScene(std::string SceneName, bool _bInstant)
{
	if (Scenes.empty())
	{
		// Scene Doesn't exist
		CLogManager::Get()->DisplayMessage("Could not find scene " + SceneName);
		return;
	}

	if (_bInstant)
	{
		// TODO: Switch to storing scene assets and using load operations
		if (!CurrentScene.empty())
		{
			const TAssetObjectPointer<Scene> ExistingScene = GetCurrentScene();
			ExistingScene->Asset->Unload();
		}
		
		CurrentScene = SceneName;
		SceneToSwitch = CurrentScene;
		CLogManager::Get()->DisplayMessage("Switching to Scene \"" + GetCurrentScene()->SceneName + "\"");
		// TODO: Switch to just load once storing assets in scene manager?
		GetCurrentScene()->Asset->Reload();
		// TODO: Move elsewhere
		GetCurrentScene()->BeginPlay();
	}
	else
	{
		SceneToSwitch = SceneName;
	}
}

/************************************************************
#--Description--#: 	Updates the current scene every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void SceneManager::UpdateCurrentScene()
{
	GetCurrentScene()->Update();
	// Switch to scene flag has been set
	if (SceneToSwitch != CurrentScene)
	{
		SwitchScene(SceneToSwitch, true);
	}
}

/************************************************************
#--Description--#:	Render Current scene to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void SceneManager::RenderCurrentScene()
{
	GetCurrentScene()->RenderScene();
}

TAssetObjectPointer<Scene> SceneManager::GetCurrentScene()
{
	if (Scenes.empty())
	{
		return nullptr;
	}
	return Scenes[CurrentScene];
};

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
THardPointer<SceneManager> SceneManager::GetInstance()
{
	if (!m_pSceneManager) // null or doesn't exist
	{
		m_pSceneManager = THardPointer<SceneManager>(new SceneManager());
	}
	return m_pSceneManager;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void SceneManager::DestoryInstance()
{
	m_pSceneManager = nullptr;
}
