// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //
#include <glm/detail/type_vec.hpp>

#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "System/Utils.h"

// OpenGL Library Includes //

class UIText;
class UIElement;
class Entity;

class ENGINE_API Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene(const std::string& InSceneName);

	virtual ~Scene();

	void DeleteScene();

	virtual void RenderScene();

	void AddEntity(TPointer<Entity> _Entity, bool IsInitial = false);

	void DestroyEntity(TPointer<Entity> _Entity);

	void AddUIElement(TPointer<UIElement> Element);

	void AddUITextElement(TPointer<UIText> Element);

	void AddUITextElement(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour, std::string _sText, std::string font, int iPSize, EANCHOR _Anchor);

	void DestroyUIElement(TPointer<UIElement> _Element);

	virtual void Update();
	virtual bool OnEvent(CEvent& Event);

	virtual void OnLoadScene();

	void SetPersistantOnLoad(bool _bIsPersistant)
	{
		bIsPersistant = _bIsPersistant;
	};

	bool operator==(const Scene& rhs) const;

	std::string SceneName;
	std::vector<TPointer<Entity>> Entities;
	std::vector<TPointer<UIElement>> UIElements;
	
protected:
	virtual bool OnMouseButtonPressedEvent(CMouseButtonPressedEvent& Event);
	virtual bool OnMouseButtonReleasedEvent(CMouseButtonReleasedEvent& Event);
	virtual bool OnMouseMovedEvent(CMouseMovedEvent& Event);
	virtual bool OnMouseScrolledEvent(CMouseScrolledEvent& Event);
	virtual bool OnKeyPressedEvent(CKeyPressedEvent& Event);
	virtual bool OnKeyReleasedEvent(CKeyReleasedEvent& Event);
	
	std::vector<TPointer<Entity>> DestroyedEntities;
	std::vector<TPointer<UIElement>> UIElementsToBeDestroyed;
	bool bIsPersistant = false;

private:
	void DestroyAllNonInitialEntities();
};
