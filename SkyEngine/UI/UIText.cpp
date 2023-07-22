// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "UIText.h"

// Library Includes //

// OpenGL Library Includes //
#include <glew/glew.h>
#include <glm/gtx/string_cast.hpp>

// Engine Includes //
#include "Text.h"
#include "Render/Shader.h"

// Local Includes //

// Static Variables //

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
UIText::UIText(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour, std::string _sText, std::string Font, int _iPSize, EANCHOR _Anchor) : UIElement(_Position, _fRotation, _Colour), sText(_sText), sFont(Font), iPSize(_iPSize), Anchor(_Anchor)
{
	Shader::AddFont(Font, iPSize);
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
UIText::UIText(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour) : UIElement(_Position, _fRotation, _Colour)
{
	sText = "";
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
UIText::~UIText()
{
}

/************************************************************
#--Description--#:	Render to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void UIText::DrawUIElement()
{
	if (!bActive || sText == "")
	{
		return;
	}
	glUseProgram(Shader::Programs["TextUIprogram"]);
	Text::Render(sText, sFont, iPSize, position, Colour, Anchor);
}
