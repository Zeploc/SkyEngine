// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Shader.h"

// Library Includes //
#include <ft2build.h>
#include <iostream>
#include <map>
#include <soil/SOIL2.h>
#include FT_FREETYPE_H

// Engine Includes //
#include "ModelObject.h"
#include "ShaderLoader.h"
#include "Core/Application.h"
#include "System/LogManager.h"

// Local Includes //

std::map<std::string, Pointer<ModelObject>> Shader::Models;
std::map<const char*, TextureData> Shader::Textures;
std::map<std::string, GLuint> Shader::Programs;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Shader::Shader()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Shader::~Shader()
{
	CleanUp();
}

void Shader::CleanUp()
{
	for (auto& Model : Models)
	{
		Model.second = nullptr;
	}
	Models.clear();
	Textures.clear();
}

void Shader::LoadAllDefaultShadersInCurrentContext()
{
	AddProgram("Resources/Shaders/TextureVertexShader.vs", "Resources/Shaders/TextureFragmentShader.fs", "BaseProgram");
	AddComputeProgram("Resources/Shaders/ComputeShader.comps", "ComputeProgram");
	AddProgram("Resources/Shaders/ParticleShaderCompute.vs", "Resources/Shaders/ParticleShaderCompute.fs", "GPUParticlesProgram");
	AddProgram("Resources/Shaders/ParticleShader.vs", "Resources/Shaders/ParticleShader.fs", "ParticleShader", "Resources/Shaders/ParticleShader.gs");
	AddProgram("Resources/Shaders/LitVertexShader.vs", "Resources/Shaders/LitFragmentShader.fs", "LitTextureprogram");
	AddProgram("Resources/Shaders/AnimatedModel.vs", "Resources/Shaders/AnimatedModel.fs", "AnimatedModel");
	AddProgram("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs", "TextUIprogram");
	AddProgram("Resources/Shaders/UI.vs", "Resources/Shaders/UI.fs", "UIprogram");
	AddProgram("Resources/Shaders/CubeMapVertexShader.vs", "Resources/Shaders/CubeMapFragmentShader.fs", "CubeMapProgram");
	AddProgram("Resources/Shaders/ModelVertexShader.vs", "Resources/Shaders/ModelFragmentShader.fs", "ModelProgram");
	AddProgram("Resources/Shaders/ModelVertexShaderLit.vs", "Resources/Shaders/ModelFragmentShaderLit.fs", "ModelProgramLit");
	AddProgram("Resources/Shaders/ReflectionShader.vs", "Resources/Shaders/ReflectionShader.fs", "ReflectionProgram");
	AddProgram("Resources/Shaders/FogShader.vs", "Resources/Shaders/FogShader.fs", "FogProgram");
	AddProgram("Resources/Shaders/GeometryShader.vs", "Resources/Shaders/GeometryShader.fs", "GeometryShader", "Resources/Shaders/GeometryShader.gs");
	AddProgram("Resources/Shaders/ParticleShader.vs", "Resources/Shaders/ParticleShader.fs", "ParticleShader", "Resources/Shaders/ParticleShader.gs");
	AddTessProgram("Resources/Shaders/Tessellation.vs", "Resources/Shaders/Tessellation.fs", "Resources/Shaders/TessControl.tc", "Resources/Shaders/TessEval.te", "TessProgram");
	AddProgram("Resources/Shaders/FrameBuffer.vs", "Resources/Shaders/FrameBuffer.fs", "FrameBuffer");
}

void Shader::AddProgram(std::string VertexShaderPath, std::string FragmentShaderPath, std::string ShaderName, std::string GeometryShaderPath)
{
	ShaderLoader loader;
	Programs.insert(std::pair<std::string, GLuint>(ShaderName, loader.CreateProgram(VertexShaderPath.c_str(), FragmentShaderPath.c_str(), GeometryShaderPath.c_str())));
	LogManager::GetInstance()->DisplayLogMessage("Loading Shader \"" + ShaderName + "\"");
}

void Shader::AddTessProgram(std::string VertexShaderPath, std::string FragmentShaderPath, std::string TessControlShaderPath, std::string TessEvalShaderPath, std::string ShaderName)
{
	ShaderLoader loader;
	Programs.insert(std::pair<std::string, GLuint>(ShaderName, loader.CreateTessProgram(VertexShaderPath.c_str(), FragmentShaderPath.c_str(), TessControlShaderPath.c_str(), TessEvalShaderPath.c_str())));
	LogManager::GetInstance()->DisplayLogMessage("Loading Shader \"" + ShaderName + "\"");
}

void Shader::AddComputeProgram(std::string ComputePath, std::string ShaderName)
{
	ShaderLoader loader;
	Programs.insert(std::pair<std::string, GLuint>(ShaderName, loader.CreateComputeProgram(ComputePath.c_str())));
	LogManager::GetInstance()->DisplayLogMessage("Loading Shader \"" + ShaderName + "\"");
}

///************************************************************
//#--Description--#: 	Binds plane
//#--Author--#: 		Alex Coultas
//#--Parameters--#: 	Takes in size and colour
//#--Return--#: 		New vao gluint
//************************************************************/
GLuint Shader::BindArray(float fWidth, float fHeight, glm::vec4 Colour)
{
	float fHalfWidth = fWidth / 2;
	float fHalfHeight = fHeight / 2;

	const std::vector<float> vertices = {
		// Positions						// Colors			
		-fHalfWidth, fHalfHeight, 0.0f, Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, fHalfHeight, 0.0f, Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, -fHalfHeight, 0.0f, Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, -fHalfHeight, 0.0f, Colour.r, Colour.g, Colour.b, Colour.a,

	};

	const std::vector<uint32_t> indices = {
		0, 1, 2, // First Triangle
		0, 2, 3 // Second Triangle
	};
	unsigned int Vao;
	GetGraphicsAPI()->BindArray(vertices, indices, Vao, true);
	return Vao;
}

/************************************************************
#--Description--#: 	Binds UI Image with texture tiling style
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in size, colour, texture source, ref texture gluint and draw mode
#--Return--#: 		New vao gluint
************************************************************/
unsigned Shader::BindUITextureArray(float fWidth, float fHeight, glm::vec4 Colour, const char* TextureSource, TextureData& Texture, int _DrawMode)
{
	unsigned int vao = GetGraphicsAPI()->CreateBuffer(TextureSource, Texture, false, false);

	glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1);
	if (_DrawMode == 1)
	{
		float yValue = (Texture.Height / Texture.Width) * (static_cast<float>(Texture.Width) / static_cast<float>(Texture.Height));
		UVCoords = glm::vec4(0, 1, 0, yValue);
	}
	else if (_DrawMode == 2)
	{
		float xValue = (Texture.Width / Texture.Height) * (static_cast<float>(Texture.Height) / static_cast<float>(Texture.Width));
		UVCoords = glm::vec4(0, xValue, 0, 1);
	}
	float fHalfWidth = fWidth / 2;
	float fHalfHeight = fHeight / 2;

	std::vector<float> vertices = {
		// Positions				// Tex Coords
		-fHalfWidth, fHalfHeight, UVCoords.x, UVCoords.z, // Top Left
		fHalfWidth, fHalfHeight, UVCoords.y, UVCoords.z, // Top Right
		fHalfWidth, -fHalfHeight, UVCoords.y, UVCoords.w, // Bottom Right
		-fHalfWidth, -fHalfHeight, UVCoords.x, UVCoords.w, // Bottom Left
	};
	std::vector<uint32_t> indices = {
		0, 1, 2, // First Triangle
		0, 2, 3 // Second Triangle
	};

	GetGraphicsAPI()->BindArray(vertices, indices, vao, false);

	return vao;
}

/************************************************************
#--Description--#: 	Adds font
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in font path and font size
#--Return--#: 		New font struct info
************************************************************/
Text::cFont Shader::AddFont(std::string fontPath, int iPSize)
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

