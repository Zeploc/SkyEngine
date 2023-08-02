// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //
#include <map>
#include <string>

// Engine Includes //
#include "Render/Texture.h"
#include "Entity/2DParticleSystem.h"
#include "UI/Text.h"

class CShader;
class ModelObject;

class ShaderManager
{
public:
	ShaderManager();

	~ShaderManager();

	static void CleanUp();

	static void LoadAllDefaultShadersInCurrentContext();

	static void AddShaderProgram(std::string ShaderName, std::string VertexShaderPath, std::string FragmentShaderPath, std::string GeometryShaderPath = "");

	static void AddTessProgram(std::string ShaderName, std::string VertexShaderPath, std::string FragmentShaderPath, std::string TessControlShaderPath, std::string TessEvalShaderPath);

	static void AddComputeProgram(std::string ShaderName, std::string ComputePath);

	static void ShaderCreated(const TPointer<CShader>NewShader);
	static TPointer<CShader> GetShader(std::string ShaderName);
	template<class T>
	static TPointer<T> GetShader();
	

	static std::map<std::string, TPointer<ModelObject>> Models;
	static std::map<std::string, CTexture> Textures;

	static unsigned int BindArray(float fWidth, float fHeight, glm::vec4 Colour);

	static unsigned int BindUITextureArray(float fWidth, float fHeight, glm::vec4 Colour, const char* TextureSource, CTexture& Texture, int _DrawMode);

	static Text::cFont AddFont(std::string fontPath, int iPSize);

protected:
	template<class T>
	static void AddShader();
	
	static std::map<std::string, TPointer<CShader>> Shaders;
};

template <class T>
void ShaderManager::AddShader()
{
	const TPointer<CShader> NewShader = std::make_shared<T>();
	ShaderCreated(NewShader);
}
