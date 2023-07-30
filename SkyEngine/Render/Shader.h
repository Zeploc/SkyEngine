// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //
#include <map>
#include <memory>
#include <string>

// OpenGL Library Includes //
#define GLEW_STATIC // This must be defined when statically linking to GLEW
#include <glew/glew.h>

// Engine Includes //
#include "UI/Text.h"

class ModelObject;

class ENGINE_API Shader
{
public:
	Shader();

	~Shader();

	static void CleanUp();

	static void LoadAllDefaultShadersInCurrentContext();

	static void AddProgram(std::string VertexShaderPath, std::string FragmentShaderPath, std::string ShaderName, std::string GeometryShaderPath = "");

	static void AddTessProgram(std::string VertexShaderPath, std::string FragmentShaderPath, std::string TessControlShaderPath, std::string TessEvalShaderPath, std::string ShaderName);

	static void AddComputeProgram(std::string ComputePath, std::string ShaderName);

	static std::map<std::string, Pointer<ModelObject>> Models;
	static std::map<const char*, GLuint> Textures;
	static std::map<std::string, GLuint> Programs;

	static GLuint BindArray(float fWidth, float fHeight, glm::vec4 Colour);

	static GLuint BindUITextureArray(float fWidth, float fHeight, glm::vec4 Colour, const char* TextureSource, GLuint& Texture, int _DrawMode);

	static Text::cFont AddFont(std::string fontPath, int iPSize);

	static GLuint CreateBuffer(const char* TextureSource, GLuint& Texture, bool bAA, bool bHasNormals = false);

	static GLuint GetTexture(const char* TextureSource, bool bAA);

private:
};
