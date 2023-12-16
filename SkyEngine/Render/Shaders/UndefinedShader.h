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
	
	struct ShaderParameters
	{
		DefineShaderVector4Param(DiffuseColour, SVector4(1.0f))
		DefineShaderTextureParam(DiffuseTexture, nullptr)
		DefineShaderFloatParam(SpecularStrength, 1.0f)
		DefineShaderFloatParam(Shininess, 32.0f)
	};

	friend std::ostream& operator<<(std::ostream& os, const ShaderParameters& InShaderParameters);
	friend std::istream& operator>>(std::istream& is, ShaderParameters& InShaderParameters);
	
	// TODO: place holder, remove
	bool HasTexture(const ShaderParameters& InParams);
	void UploadMaterialParameters(const ShaderParameters& InParams);
};
