//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    UIText.cpp
// Description    	:    main implementation for UIText
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "UIText.h"

// Library Includes //

// OpenGL Library Includes //
#include <glm\gtx\string_cast.hpp>
#include <glew.h>

// Engine Includes //
#include "Text.h"
#include "Engine/Render/Shader.h"

// Local Includes //


// Static Variables //


/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
UIText::UIText(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour, std::string _sText, std::string Font, int _iPSize, Utils::EANCHOR _Anchor)
	: UIElement(_Position, _fRotation, _Colour), sText(_sText), sFont(Font), iPSize(_iPSize), Anchor(_Anchor)
{
	Shader::AddFont(Font, iPSize);
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
UIText::UIText(glm::vec2 _Position, float _fRotation, glm::vec4 _Colour)
	: UIElement(_Position, _fRotation, _Colour)
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
		return;
	glUseProgram(Shader::Programs["TextUIprogram"]);
	Text::Render(sText, sFont, iPSize, position, Colour, Anchor);
}