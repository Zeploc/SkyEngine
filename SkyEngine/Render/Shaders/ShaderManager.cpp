// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "ShaderManager.h"

// Library Includes //
#include <ft2build.h>
#include <soil/SOIL2.h>
#include FT_FREETYPE_H

// Engine Includes //
#include "PBRShader.h"
#include "Shader.h"
#include "UndefinedShader.h"
#include "UnlitShader.h"
#include "Render/Meshes/Model/ModelObject.h"
#include "Core/Application.h"
#include "System/LogManager.h"
#include "UndefinedShader.h"
#include "UI/Legacy/Text.h"

// Local Includes //

std::map<std::string, TPointer<ModelObject>> ShaderManager::Models;
std::map<std::string, TPointer<CTexture>> ShaderManager::Textures;
std::map<std::string, TPointer<CShader>> ShaderManager::Shaders;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
ShaderManager::ShaderManager()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
ShaderManager::~ShaderManager()
{
	CleanUp();
}

void ShaderManager::CleanUp()
{
	for (auto& Model : Models)
	{
		Model.second = nullptr;
	}
	Models.clear();
	Textures.clear();
}

void ShaderManager::LoadAllDefaultShaders()
{
	/*
	AddShaderProgram("BaseProgram", "Resources/Shaders/TextureVertexShader.vs", "Resources/Shaders/TextureFragmentShader.fs");
	AddComputeProgram("ComputeProgram", "Resources/Shaders/ComputeShader.comps");
	AddShaderProgram("GPUParticlesProgram", "Resources/Shaders/ParticleShaderCompute.vs", "Resources/Shaders/ParticleShaderCompute.fs");
	AddShaderProgram("ParticleShader", "Resources/Shaders/ParticleShader.vs", "Resources/Shaders/ParticleShader.fs", "Resources/Shaders/ParticleShader.gs");
	AddShaderProgram("LitTextureprogram", "Resources/Shaders/LitVertexShader.vs", "Resources/Shaders/LitFragmentShader.fs");
	AddShaderProgram("AnimatedModel", "Resources/Shaders/AnimatedModel.vs", "Resources/Shaders/AnimatedModel.fs");
	AddShaderProgram("TextUIprogram", "Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs");
	AddShaderProgram("UIprogram", "Resources/Shaders/UI.vs", "Resources/Shaders/UI.fs");
	AddShaderProgram("CubeMapProgram", "Resources/Shaders/CubeMapVertexShader.vs", "Resources/Shaders/CubeMapFragmentShader.fs");
	AddShaderProgram("ModelProgram", "Resources/Shaders/ModelVertexShader.vs", "Resources/Shaders/ModelFragmentShader.fs");
	AddShaderProgram("ModelProgramLit", "Resources/Shaders/ModelVertexShaderLit.vs", "Resources/Shaders/ModelFragmentShaderLit.fs");
	AddShaderProgram("ReflectionProgram", "Resources/Shaders/ReflectionShader.vs", "Resources/Shaders/ReflectionShader.fs");
	AddShaderProgram("FogProgram", "Resources/Shaders/FogShader.vs", "Resources/Shaders/FogShader.fs");
	AddShaderProgram("GeometryShader", "Resources/Shaders/GeometryShader.vs", "Resources/Shaders/GeometryShader.fs", "Resources/Shaders/GeometryShader.gs");
	AddShaderProgram("ParticleShader", "Resources/Shaders/ParticleShader.vs", "Resources/Shaders/ParticleShader.fs", "Resources/Shaders/ParticleShader.gs");
	AddTessProgram("TessProgram", "Resources/Shaders/Tessellation.vs", "Resources/Shaders/Tessellation.fs", "Resources/Shaders/TessControl.tc", "Resources/Shaders/TessEval.te");
	AddShaderProgram("FrameBuffer", "Resources/Shaders/FrameBuffer.vs", "Resources/Shaders/FrameBuffer.fs");
	*/

	AddShader<CPBRShader>();
	AddShader<CUnlitShader>();
}

void ShaderManager::AddShaderProgram(std::string ShaderName, std::string VertexShaderPath, std::string FragmentShaderPath, std::string GeometryShaderPath)
{
	const TPointer<CUndefinedShader> NewShader = std::make_shared<CUndefinedShader>(ShaderName, VertexShaderPath, FragmentShaderPath);
	NewShader->SetGeometryShader(GeometryShaderPath);
	ShaderCreated(NewShader);
}

void ShaderManager::AddTessProgram(std::string ShaderName, std::string VertexShaderPath, std::string FragmentShaderPath, std::string TessControlShaderPath, std::string TessEvalShaderPath)
{
	const TPointer<CUndefinedShader> NewShader = std::make_shared<CUndefinedShader>(ShaderName, VertexShaderPath, FragmentShaderPath);
	NewShader->SetTessShader(TessControlShaderPath, TessEvalShaderPath);
	ShaderCreated(NewShader);
}

void ShaderManager::AddComputeProgram(std::string ShaderName, std::string ComputePath)
{
	const TPointer<CUndefinedShader> NewShader = std::make_shared<CUndefinedShader>(ShaderName, ComputePath);
	ShaderCreated(NewShader);
}

void ShaderManager::ShaderCreated(const TPointer<CShader>NewShader)
{
	std::string ShaderName = NewShader->GetShaderName();
	if (!NewShader->CompileShader())
	{
		const std::string ErrorMessage = "Failed to create shader" + ShaderName;
		ensure(false, ErrorMessage.c_str());
		return;
	}
	Shaders.insert(std::pair(ShaderName, NewShader));
	CLogManager::Get()->DisplayMessage("Loading Shader \"" + ShaderName + "\"");
}

TPointer<CShader> ShaderManager::GetShader(std::string ShaderName)
{
	if (!Shaders.contains(ShaderName))
	{
		return nullptr;
	}
	return Shaders[ShaderName];
}

TPointer<CUndefinedShader> ShaderManager::GetUndefinedShader(std::string ShaderName)
{
	if (!Shaders.contains(ShaderName))
	{
		return nullptr;
	}
	return std::static_pointer_cast<CUndefinedShader>(Shaders[ShaderName]);
}

/************************************************************
#--Description--#: 	Adds font
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in font path and font size
#--Return--#: 		New font struct info
************************************************************/
Text::cFont ShaderManager::AddFont(std::string fontPath, int iPSize)
{
	for (auto it : Text::Fonts)
	{
		if (it.sPathName == fontPath && it.fPSize == iPSize)
		{
			return it;
		}
	}

	// Initiate the font Lib
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	// Each character is called a glyph and Face is the collection of glyphs
	FT_Face face;
	// Load font as face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to Load font" << std::endl;
	}
	// Set size to load glyph as
	FT_Set_Pixel_Sizes(face, 0, iPSize);
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	Text::cFont newFont;
	newFont.sPathName = fontPath;
	newFont.fPSize = iPSize;
	// Load the first 128 characters of the ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load the character glyph into face
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture for character
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_ALPHA,
		             GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now to store character for later use
		Text::Character character = {
			texture,
			glm::ivec2(static_cast<GLuint>(face->glyph->bitmap.width), static_cast<GLuint>(face->glyph->bitmap.rows)),
			glm::ivec2(static_cast<GLuint>(face->glyph->bitmap_left), static_cast<GLuint>(face->glyph->bitmap_top)),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		newFont.Characters.insert(std::pair<GLchar, Text::Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we are finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &newFont.VAO);
	glBindVertexArray(newFont.VAO);

	glGenBuffers(1, &newFont.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newFont.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Text::Fonts.push_back(newFont);
	return newFont;
}

std::map<std::string, TPointer<CShader>> ShaderManager::GetShaders()
{
	return Shaders;
}

