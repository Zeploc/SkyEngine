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
		DefineShaderParameter(DiffuseColour);
		DefineShaderParameter(DiffuseTexture);
		DefineShaderParameter(SpecularStrength);
		DefineShaderParameter(Shininess);
	ListShaderVariables()
		SVector4 DiffuseColour = SVector4(1.0f);
		TPointer<CTexture> DiffuseTexture = nullptr;
		float SpecularStrength = 1.0f;
		float Shininess = 32.0f;
	EndShaderParams()
	
	// TODO: place holder, remove
	bool HasTexture(const ShaderParameters& InParams);
};
