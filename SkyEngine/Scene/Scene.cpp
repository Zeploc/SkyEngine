// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Scene.h"

// Only used for key inputs, swap out later
#include <glew/glew.h>

// Engine Includes //
#include "Entity/Entity.h"

// Local Includes //

// This Includes //

#include "Core/Application.h"
#include "Entity/Button3DEntity.h"
#include "Render/Renderer.h"
#include "Input/Input.h"
#include "Platform/Window/GLFW/GLFWIncludes.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Scene::Scene(const std::string& InSceneName) : SceneName(InSceneName)
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Scene::~Scene()
{
	DeleteScene();
}

/************************************************************
#--Description--#:	Delete all entities in scene
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Scene::DeleteScene()
{
	for (auto it : Entities)
	{
		it = nullptr;
	}
}

/************************************************************
#--Description--#:	Render Scene to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Scene::RenderScene()
{
	GetRenderer()->Render(Entities);
}

/************************************************************
#--Description--#:	Add entity to scene
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Entity to add
#--Return--#: 		NA
************************************************************/
void Scene::AddEntity(TPointer<Entity> _Entity, bool IsInitial)
{
	Entities.push_back(_Entity);
	_Entity->SetInitialEntity(IsInitial);
}

/************************************************************
#--Description--#:	Destroys entity
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Entity to destroy
#--Return--#: 		NA
************************************************************/
void Scene::DestroyEntity(TPointer<Entity> _Entity)
{
	if (_Entity->IsInitialEntity())
	{
		DestroyedEntities.push_back(_Entity);
		_Entity->SetActive(false);
	}
	_Entity->OnDestroy();

	// Find entity in entities
	for (auto it = Entities.begin(); it != Entities.end(); ++it)
	{
		if (*it == _Entity || *it == nullptr)
		{
			// Remove from entities list
			Entities.erase(it);
			break;
		}
	}
	// Reset all but this option?
	//EntDetroy.reset();
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Scene::Update()
{	
	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities[i])
		{
			Entities[i]->BaseUpdate();
		}
	}

	Button3DEntity::bButtonPressedThisFrame = false;	
}

void Scene::OnLoadScene()
{
	if (!bIsPersistant)
	{
		for (auto& EntDestroy : DestroyedEntities)
		{
			Entities.push_back(EntDestroy);
		}
		DestroyedEntities.clear();
		DestroyAllNonInitialEntities();
		for (auto& Ent : Entities)
		{
			Ent->Reset();
		}
	}
}

/************************************************************
#--Description--#: 	== operator overloading
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Comparing scene
#--Return--#: 		NA
************************************************************/
bool Scene::operator==(const Scene& rhs) const
{
	if (SceneName == rhs.SceneName)
	{
		return true;
	}
	return false;
}

bool Scene::OnMouseButtonPressed(int Button, int Mods)
{
	return false;
}

bool Scene::OnMouseButtonReleased(int Button, int Mods)
{
	return false;
}

bool Scene::OnMouseMoved(SVector2i MousePos)
{
	return false;
}

bool Scene::OnMouseScrolled(float XOffset, float YOffset)
{
	return false;
}

bool Scene::OnKeyPressed(int KeyCode, int Mods, int RepeatCount)
{
	// TODO: If not build check (editor only)
	if (KeyCode == GLFW_KEY_ESCAPE) 
	{
		if (KeyCode & CWindowInput::ModiferType::Shift)
		{
			GetApplication()->Quit();
			return true;
		}
	}
	return false;
}

bool Scene::OnKeyReleased(int KeyCode, int Mods)
{
	return false;
}

void Scene::DestroyAllNonInitialEntities()
{
	auto EndIt = Entities.end();
	for (auto it = Entities.begin(); it != EndIt; ++it)
	{
		if (!(*it)->IsInitialEntity())
		{
			b2Body* EntBody = (*it)->body;
			if (EntBody)
			{
				EntBody->GetWorld()->DestroyBody(EntBody);
			}
			it = Entities.erase(it);
			--it;
			EndIt = Entities.end();
		}
	}
}
