// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <string>

// OpenGL Library Includes //

// Engine Includes //
#include "UIElement.h"
#include "UIImage.h"
#include "UIText.h"
#include "Engine/System/Delegates.h"

class UIButton : public UIElement
{
public:
	UIButton(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, glm::vec4 _Colour, glm::vec4 _HightlightColour, int iWidth, int iHeight, void (*func)() = nullptr);

	UIButton(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, const char* _ImagePath, glm::vec4 _HightlightColour, int iWidth, int iHeight, void (*func)() = nullptr);

	~UIButton();

	void AddText(std::string sText, std::string sFont, int iPSize, glm::vec4 TextColour, Utils::EANCHOR _Anchor);

	void AddText(std::string sText, std::string sFont, int iPSize, glm::vec4 TextColour, Utils::EANCHOR _Anchor, glm::vec2 _v2Offset);

	template <class T>
	void BindPress(T* Object, void(T::*Function)())
	{
		PressDelegate.Bind(Object, Function);
	}
	
	template <class T>
	void UnbindPress(T* Object, void(T::*Function)())
	{
		PressDelegate.Unbind(Object, Function);
	}

	template <class T>
	void BindHold(T* Object, void(T::*Function)())
	{
		HoldDelegate.Bind(Object, Function);
	}

	template <class T>
	void BindRelease(T* Object, void(T::*Function)())
	{
		ReleaseDelegate.Bind(Object, Function);
	}

	void SetActive(bool _bIsActive) override;

	void SetPressSound(const char* _SoundPath);

	UIText TextComponent;
	UIImage ImageComponent;

	void DrawUIElement() override;

	void Update() override;

	void SetPosition(glm::vec2 _NewPosition) override;

	void Pressed();

	void Hovered();

	bool HoverOverride = false;

	bool bPressed = false;

	static bool bButtonPressedThisFrame;
	
	FMulticastDelegate PressDelegate;
	FMulticastDelegate HoldDelegate;
	FMulticastDelegate ReleaseDelegate;

private:
	bool bHasText = false;

	glm::vec4 btnColour;
	glm::vec4 btnHighlightColour;

	const char* m_PressSoundPath = "";
};
