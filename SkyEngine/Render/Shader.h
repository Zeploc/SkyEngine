// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //
#include <map>
#include <string>

// Engine Includes //
#include "Texture.h"
#include "Entity/2DParticleSystem.h"
#include "Entity/2DParticleSystem.h"
#include "Entity/2DParticleSystem.h"
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
	static std::map<const char*, TextureData> Textures;
	static std::map<std::string, unsigned int> Programs;

	static unsigned int BindArray(float fWidth, float fHeight, glm::vec4 Colour);

	static unsigned int BindUITextureArray(float fWidth, float fHeight, glm::vec4 Colour, const char* TextureSource, TextureData& Texture, int _DrawMode);

	static Text::cFont AddFont(std::string fontPath, int iPSize);
private:
};
