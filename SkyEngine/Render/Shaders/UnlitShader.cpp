// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UnlitShader.h"

#include "Core/Application.h"


CUnlitShader::CUnlitShader()
: CShader(GetStaticName(), "Resources/Shaders/UnlitVertexShader.vs", "Resources/Shaders/UnlitFragmentShader.fs")
{
	// TODO: Setup shader uniforms
}

std::string CUnlitShader::GetStaticName()
{
	return "Unlit";
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

std::ostream& operator<<(std::ostream& os, const CUnlitShader::ShaderParameters& InShaderParameters)
{
	InShaderParameters.SerializeDiffuseColour(os);
	InShaderParameters.SerializeDiffuseTexture(os);
	return os;
}

std::istream& operator>>(std::istream& is, CUnlitShader::ShaderParameters& InShaderParameters)
{
	InShaderParameters.DeserializeDiffuseColour(is);
	InShaderParameters.DeserializeDiffuseTexture(is);
	return is;
}
