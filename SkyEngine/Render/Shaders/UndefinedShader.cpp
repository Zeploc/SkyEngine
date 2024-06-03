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