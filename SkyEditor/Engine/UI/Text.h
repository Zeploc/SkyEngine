// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <map>
#include <string>
#include <vector>

// OpenGL Library Includes //
#include <glew/glew.h>

// Engine Includes //
#include "Engine/System/Utils.h"

class Text
{
public:
	struct Character
	{
		GLuint TextureID; //Texture ID
		glm::ivec2 Size; // Size of glyph
		glm::ivec2 Bearing; // Position of glyph
		GLuint Advance; // How far to move for the next character
	};

	struct cFont
	{
		std::map<GLchar, Character> Characters;
		std::string sPathName;
		GLuint VAO;
		GLuint VBO;
		int fPSize;
	};

	static std::vector<cFont> Fonts;

	static void Render(std::string Text, std::string Font, int iSize, glm::vec2 Position, glm::vec3 colour, EANCHOR _Anchor);
};
