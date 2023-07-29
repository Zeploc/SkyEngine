// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //
#include <memory>
#include <string>
#include <vector>
#include <glm/detail/type_vec.hpp>

#include "System/Utils.h"

// OpenGL Library Includes //

class UIText;
class UIElement;
class Entity;

class ENGINE_API Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene(std::string sSceneName);

	virtual ~Scene();

	void DeleteScene();

	virtual void RenderScene();

	void AddEntity(Pointer<Entity> _Entity, bool IsInitial = false);

	void DestroyEntity(Pointer<Entity> _Entity);

	void AddUIElement(Pointer<UIElement> Element);

	void AddUITextElement(Pointer<UIText> Element);

	void AddUITextElement(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour, std::string _sText, std::string font, int iPSize, EANCHOR _Anchor);

	void DestroyUIElement(Pointer<UIElement> _Element);

	virtual void Update();

	virtual void OnLoadScene();

	void SetPersistantOnLoad(bool _bIsPersistant)
	{
		bIsPersistant = _bIsPersistant;
	};

	bool operator==(const Scene& rhs) const;

	std::string SceneName;
	std::vector<Pointer<Entity>> Entities;
	std::vector<Pointer<UIElement>> UIElements;
protected:
	std::vector<Pointer<Entity>> DestroyedEntities;
	std::vector<Pointer<UIElement>> UIElementsToBeDestroyed;
	bool bIsPersistant = false;

private:
	void DestroyAllNonInitialEntities();
};
