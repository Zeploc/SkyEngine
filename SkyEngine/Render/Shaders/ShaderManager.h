// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //

// Engine Includes //
#include "Shader.h"
#include "UI/Legacy/Text.h"
// #include "Entity/2DParticleSystem.h"

// class CShader;
class ModelObject;
class CUndefinedShader;

class ENGINE_API ShaderManager
{
public:
	ShaderManager();

	~ShaderManager();

	static void CleanUp();

	static void LoadAllDefaultShaders();

	static void AddShaderProgram(std::string ShaderName, std::string VertexShaderPath, std::string FragmentShaderPath, std::string GeometryShaderPath = "");

	static void AddTessProgram(std::string ShaderName, std::string VertexShaderPath, std::string FragmentShaderPath, std::string TessControlShaderPath, std::string TessEvalShaderPath);

	static void AddComputeProgram(std::string ShaderName, std::string ComputePath);

	static void ShaderCreated(const TPointer<CShader>NewShader);
	static TPointer<CShader> GetShader(std::string ShaderName);
	static TPointer<CUndefinedShader> GetUndefinedShader(std::string ShaderName);
	template<class T>
	static TPointer<T> GetShader();	

	static std::map<std::string, TPointer<ModelObject>> Models;

	static Text::cFont AddFont(std::string fontPath, int iPSize);
	
	// TODO: Needed for template as static shaders can't be exported :( 
	static std::map<std::string, TPointer<CShader>> GetShaders();
	
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

template <class T>
TPointer<T> ShaderManager::GetShader()
{
	const std::map<std::string, TPointer<CShader>> CurrentShaders = GetShaders();
	// TODO: improve retrieval based on stored method
	for (auto Element : CurrentShaders)
	{
		TPointer<CShader> Shader = Element.second;
		if (TPointer<T> CheckedShader = Cast<T>(Shader))
		{
			return CheckedShader;
		}				
	}
	return nullptr;
}