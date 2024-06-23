// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UnlitShader.h"


CUnlitShader::CUnlitShader()
: CShader(GetStaticName(), "Resources/Shaders/UnlitVertexShader.vs", "Resources/Shaders/UnlitFragmentShader.fs")
{
}

std::string CUnlitShader::GetStaticName()
{
	return "Unlit";
}

bool CUnlitShader::HasTexture(const ShaderParameters& InParams)
{
	return InParams.DiffuseTexture != nullptr;
}