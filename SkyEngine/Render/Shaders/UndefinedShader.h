// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Shader.h"
#include "Math/Vector4.h"

class CTexture;

class ENGINE_API CUndefinedShader : public CShader
{
public:
	CUndefinedShader(const std::string& InShaderName, const std::string& InVertexShaderPath, const std::string& InFragmentShaderPath);
	~CUndefinedShader() override;
	
	using CShader::CShader;
	
	BeginShaderParams()
	ListShaderVariables()
	EndShaderParams()
	
	// TODO: place holder, remove
	bool HasTexture(const ShaderParameters& InParams);
};
