// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// OpenGL Library Includes //
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API UIElement : public std::enable_shared_from_this<UIElement>
{
public:
	UIElement();

	UIElement(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour);

	~UIElement();

	virtual void DrawUIElement();

	void BaseUpdate();

	virtual void Update();

	virtual void SetPosition(glm::vec2 _NewPosition);

	void Rotate(float fAngle);

	void SetRotation(float _fRotation);

	virtual void SetActive(bool _bIsActive);

	bool IsActive() { return bActive; };

	glm::vec2 GetPosition();

	float fRotation;
	glm::vec4 Colour;

protected:
	bool bActive = true;
	glm::vec2 position;
};
