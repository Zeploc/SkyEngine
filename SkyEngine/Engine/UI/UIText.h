// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "UIElement.h"

// Library Includes //
#include <string>

// OpenGL Library Includes //

// Engine Includes //
#include "Engine/System/Utils.h"

class UIText : public UIElement
{
public:
	void DrawUIElement() override;

	UIText(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour, std::string _sText, std::string Font, int _iPSize, EANCHOR _Anchor);

	UIText(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour);

	~UIText();

	std::string sText;
	std::string sFont;
	int iPSize;
	EANCHOR Anchor;
};
