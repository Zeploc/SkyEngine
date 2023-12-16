// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UndefinedShader.h"


CUndefinedShader::CUndefinedShader(const std::string& InShaderName, const std::string& InVertexShaderPath, const std::string& InFragmentShaderPath)
	:CShader(InShaderName, InVertexShaderPath, InFragmentShaderPath)
{
}

CUndefinedShader::~CUndefinedShader()
{
}

bool CUndefinedShader::HasTexture(const ShaderParameters& InParams)
{
	return false;
}

void CUndefinedShader::UploadMaterialParameters(const ShaderParameters& InParams)
{
}

std::ostream& operator<<(std::ostream& os, const CUndefinedShader::ShaderParameters& InShaderParameters)
{
	return os;
}

std::istream& operator>>(std::istream& is, CUndefinedShader::ShaderParameters& InShaderParameters)
{
	return is;
}
