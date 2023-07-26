// Copyright Skyward Studios, Inc. All Rights Reserved.

#include <glew/glew.h>

// Engine Includes //
#include "Entity/Entity.h"
#include "UI/UIElement.h"
#include "UI/UIText.h"

// Local Includes //

// This Includes //
#include "Scene.h"

#include <GLFW/glfw3.h>

#include "Camera/CameraManager.h"
#include "Core/Application.h"
#include "Entity/Button3DEntity.h"
#include "Graphics/GraphicsWindow.h"
#include "Input/Input.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Scene::Scene(std::string sSceneName) : SceneName(sSceneName)
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
	for (auto it : UIElements)
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
	// TODO: Properly link to graphics interface
	GetApplication()->GetApplicationWindow()->GetGraphicsWindow()->Render(Entities, UIElements);
}

/************************************************************
#--Description--#:	Add entity to scene
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Entity to add
#--Return--#: 		NA
************************************************************/
void Scene::AddEntity(std::shared_ptr<Entity> _Entity, bool IsInitial)
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
void Scene::DestroyEntity(std::shared_ptr<Entity> _Entity)
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
#--Description--#:	Adds UI element
#--Author--#: 		Alex Coultas
#--Parameters--#: 	UI Element to add
#--Return--#: 		NA
************************************************************/
void Scene::AddUIElement(std::shared_ptr<UIElement> Element)
{
	UIElements.push_back(Element);
}

/************************************************************
#--Description--#:	Adds UI Text element
#--Author--#: 		Alex Coultas
#--Parameters--#: 	UI Text Element to add
#--Return--#: 		NA
************************************************************/
void Scene::AddUITextElement(std::shared_ptr<UIText> Element)
{
	UIElements.push_back(Element);
}

/************************************************************
#--Description--#:	Adds UI Text element
#--Author--#: 		Alex Coultas
#--Parameters--#: 	UI Text Element paramters to add
#--Return--#: 		NA
************************************************************/
void Scene::AddUITextElement(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour, std::string _sText, std::string font, int iPSize, EANCHOR _Anchor)
{
	std::shared_ptr<UIText> NewElement(new UIText(_Position, _fRotation, _Colour, _sText, font, iPSize, _Anchor));
	UIElements.push_back(NewElement);
}

/************************************************************
#--Description--#:	Destroys UI Text element
#--Author--#: 		Alex Coultas
#--Parameters--#: 	UI Text Element paramters to destroy
#--Return--#: 		NA
************************************************************/
void Scene::DestroyUIElement(std::shared_ptr<UIElement> _Element)
{
	UIElementsToBeDestroyed.push_back(_Element);
	_Element->SetActive(false);
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Scene::Update()
{
	// TODO: Move to better location?
	CameraManager* CameraInstance = CameraManager::GetInstance();
	CameraInstance->SpectatorUpdate();
	
	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities[i])
		{
			Entities[i]->BaseUpdate();
		}
	}

	for (int i = 0; i < UIElements.size(); i++)
	{
		if (UIElements[i])
		{
			UIElements[i]->BaseUpdate();
		}
	}

	for (auto& UIDestroy : UIElementsToBeDestroyed)
	{
		for (auto it = UIElements.begin(); it != UIElements.end(); ++it)
		{
			if (*it == UIDestroy)
			{
				UIElements.erase(it);
				break;
			}
		}
		UIDestroy.reset();
	}
	if (!UIElementsToBeDestroyed.empty())
	{
		UIElementsToBeDestroyed.clear();
	}

	Button3DEntity::bButtonPressedThisFrame = false;

	// TODO: If not build check (editor only)
	if (Input::GetInstance()->KeyState[GLFW_KEY_ESCAPE] == Input::INPUT_FIRST_PRESS) // Escape
	{
		if (Input::GetInstance()->KeyState[GLFW_KEY_LEFT_SHIFT] == Input::InputState::INPUT_HOLD)
		{
			GetApplication()->Quit();
			return;
		}
	}
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
