// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UnlitShader.h"

#include <glew/glew.h>

#include "Core/Application.h"


CUnlitShader::CUnlitShader()
: CShader("Unlit","Resources/Shaders/UnlitVertexShader.vs", "Resources/Shaders/UnlitFragmentShader.fs")
{
	// TODO: Setup shader uniforms
}

bool CUnlitShader::CompileShader()
{
	if (!CShader::CompileShader())
	{
		return false;
	}
	Params.DiffuseTextureLocation = GetAttributeLocation(Params.DiffuseTextureName);
	Params.HasDiffuseTextureLocation = GetAttributeLocation(std::string("bHas") + Params.DiffuseTextureName);
	Params.DiffuseColourLocation = GetAttributeLocation(Params.DiffuseColourName);

	return true;
}

void CUnlitShader::BindShader()
{
	CShader::BindShader();
}

void CUnlitShader::UploadMaterialParameters(const ShaderParameters& InParams)
{
	GetGraphicsAPI()->PassAttributeToShader(Params.DiffuseTextureLocation, InParams.DiffuseTexture);
	const bool bHasTexture = InParams.DiffuseTexture != nullptr;
	GetGraphicsAPI()->PassAttributeToShader(Params.HasDiffuseTextureLocation, bHasTexture);
	GetGraphicsAPI()->PassAttributeToShader(Params.DiffuseColourLocation, InParams.DiffuseColour);
}

bool CUnlitShader::HasTexture(const ShaderParameters& InParams)
{
	return InParams.DiffuseTexture != nullptr;
}
