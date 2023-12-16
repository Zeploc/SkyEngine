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
TPointer<SceneManager> SceneManager::m_pSceneManager;

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
	for (auto it : Scenes)
	{
		it.second->DeleteScene();
		it.second = nullptr;
	}
}

/************************************************************
#--Description--#: 	Adds scene to scene manager
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Scene to add
#--Return--#: 		NA
************************************************************/
void SceneManager::AddScene(TPointer<Scene> _Scene)
{
	if (Scenes.contains(_Scene->SceneName))
	{
		ensure(false, "Attempted to add scene with same name as existing!");
		return;
	}
	if (Scenes.empty())
	{
		Scenes.insert(std::pair<std::string, TPointer<Scene>>(_Scene->SceneName, _Scene));

		SceneToSwitch = _Scene->SceneName;
		SwitchScene(SceneToSwitch, true);
	}
	else
	{
		Scenes.insert(std::pair<std::string, TPointer<Scene>>(_Scene->SceneName, _Scene));
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
	if (Scenes.count(SceneName) == 0)
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
void SceneManager::RemoveScene(TPointer<Scene> _Scene)
{
	for (auto it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		if ((*it).second == _Scene)
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
			const TPointer<Scene> ExistingScene = GetCurrentScene();
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

TPointer<Scene> SceneManager::GetCurrentScene()
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
TPointer<SceneManager> SceneManager::GetInstance()
{
	if (!m_pSceneManager) // null or doesn't exist
	{
		m_pSceneManager = TPointer<SceneManager>(new SceneManager());
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
