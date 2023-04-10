// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Local Includes //
#include "UIElement.h"

// Engine Includes //
#include "UIImage.h"
#include "UIText.h"

class UITextField : public UIElement
{
public:
	UITextField(glm::vec2 _Position, EANCHOR _anchor, float _fRotation, glm::vec4 _Colour, int iWidth, int iHeight, glm::vec4 _TextColour, std::string _DefaultValue, std::string _FontPath, int _iPSize, EANCHOR TextAnchor);

	UITextField(glm::vec2 _Position, EANCHOR _anchor, float _fRotation, const char* _ImagePath, int iWidth, int iHeight, glm::vec4 _TextColour, std::string _DefaultValue, std::string _FontPath, int _iPSize, EANCHOR TextAnchor);

	~UITextField();

	void SetActive(bool _bIsActive) override;

	void ResetField();

	void DrawUIElement() override;

	void Update() override;

	void SetFocussed(bool bNewFocus);

	bool GetFocussed() { return bIsFocussed; };

	UIText FieldText;
	UIImage BackImage;

	std::string sHintText;

private:
	bool bIsFocussed = false;
	bool bHintTextActive = true;

	double dSpamDelay = 0.0f;
	float fSpamTime = 0.1f;
};
