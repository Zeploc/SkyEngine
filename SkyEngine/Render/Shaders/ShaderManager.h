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

	static void ShaderCreated(const THardPointer<CShader>NewShader);
	static THardPointer<CShader> GetShader(std::string ShaderName);
	static THardPointer<CUndefinedShader> GetUndefinedShader(std::string ShaderName);
	template<class T>
	static THardPointer<T> GetShader();	

	static std::map<std::string, THardPointer<ModelObject>> Models;

	static Text::cFont AddFont(std::string fontPath, int iPSize);
	
	// TODO: Needed for template as static shaders can't be exported :( 
	static std::map<std::string, THardPointer<CShader>> GetShaders();
	
protected:
	template<class T>
	static void AddShader();
	
	static std::map<std::string, THardPointer<CShader>> Shaders;
};

template <class T>
void ShaderManager::AddShader()
{
	const THardPointer<CShader> NewShader = std::make_shared<T>();
	ShaderCreated(NewShader);
}

template <class T>
THardPointer<T> ShaderManager::GetShader()
{
	const std::map<std::string, THardPointer<CShader>> CurrentShaders = GetShaders();
	// TODO: improve retrieval based on stored method
	for (auto Element : CurrentShaders)
	{
		THardPointer<CShader> Shader = Element.second;
		if (THardPointer<T> CheckedShader = Cast<T>(Shader))
		{
			return CheckedShader;
		}				
	}
	return nullptr;
}