// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Entity.h"
#include "Engine/UI/UIButton.h"

class Button3DEntity : public Entity
{
public:
	Button3DEntity(Utils::Transform _Transform, Utils::EANCHOR _Anchor, float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, glm::vec4 _HightlightColour, void (*func)() = nullptr);

	Button3DEntity(Utils::Transform _Transform, Utils::EANCHOR _Anchor, float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, glm::vec4 _HightlightColour, const char* Texturepath, void (*func)() = nullptr);

	~Button3DEntity();

	template <class T>
	void BindPress(FDelegate<T>* callback)
	{
		PressFunc = callback;
	}

	void Update() override;

	bool bPressed = false;

	static bool bButtonPressedThisFrame;

private:
	using GMVoidFunc = void(*)();

	DelegateBase* PressFunc;
	GMVoidFunc PressFuncCall = nullptr;

	bool CheckHit(glm::vec3 BottomLeftOffset, glm::vec3 TopRightOffset);

	glm::vec4 btnColour;
	glm::vec4 btnHighlightColour;
};
