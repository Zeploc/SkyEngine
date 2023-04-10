// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Entity.h"
#include "Engine/UI/UIButton.h"

class Button3DEntity : public Entity
{
public:
	Button3DEntity(FTransform _Transform, EANCHOR _Anchor, float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, glm::vec4 _HightlightColour, void (*func)() = nullptr);

	Button3DEntity(FTransform _Transform, EANCHOR _Anchor, float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, glm::vec4 _HightlightColour, const char* Texturepath, void (*func)() = nullptr);

	~Button3DEntity();

	template <class T>
	void BindPress(T* Object, void(T::*Function)())
	{
		PressDelegate.Bind(Object, Function);
	}

	void Update() override;

	bool bPressed = false;

	static bool bButtonPressedThisFrame;

	FMulticastDelegate PressDelegate;

private:

	bool CheckHit(glm::vec3 BottomLeftOffset, glm::vec3 TopRightOffset);

	glm::vec4 btnColour;
	glm::vec4 btnHighlightColour;
};
