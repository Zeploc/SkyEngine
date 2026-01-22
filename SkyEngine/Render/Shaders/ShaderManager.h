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

	static void ShaderCreated(const TSharedPointer<CShader>NewShader);
	static TSharedPointer<CShader> GetShader(std::string ShaderName);
	static TSharedPointer<CUndefinedShader> GetUndefinedShader(std::string ShaderName);
	template<class T>
	static TSharedPointer<T> GetShader();	

	static std::map<std::string, TSharedPointer<ModelObject>> Models;

	static Text::cFont AddFont(std::string fontPath, int iPSize);
	
	// TODO: Needed for template as static shaders can't be exported :( 
	static std::map<std::string, TSharedPointer<CShader>> GetShaders();
	
protected:
	template<class T>
	static void AddShader();
	
	static std::map<std::string, TSharedPointer<CShader>> Shaders;
};

template <class T>
void ShaderManager::AddShader()
{
	const TSharedPointer<CShader> NewShader = std::make_shared<T>();
	ShaderCreated(NewShader);
}

template <class T>
TSharedPointer<T> ShaderManager::GetShader()
{
	const std::map<std::string, TSharedPointer<CShader>> CurrentShaders = GetShaders();
	// TODO: improve retrieval based on stored method
	for (auto Element : CurrentShaders)
	{
		TSharedPointer<CShader> Shader = Element.second;
		if (TSharedPointer<T> CheckedShader = Cast<T>(Shader))
		{
			return CheckedShader;
		}				
	}
	return nullptr;
}