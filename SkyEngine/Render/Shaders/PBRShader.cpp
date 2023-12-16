// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "PBRShader.h"

#include "Graphics/GL/GLIncludes.h"

#include "Core/Application.h"
#include "Render/Lighting.h"

CPBRShader::CPBRShader()
: CShader(GetStaticName(),"Resources/Shaders/PBRVertexShader.vs", "Resources/Shaders/PBRFragmentShader.fs")
{
}

CPBRShader::~CPBRShader()
{
}

std::string CPBRShader::GetStaticName()
{
	return "PBR";
}

bool CPBRShader::CompileShader()
{
	if (!CShader::CompileShader())
	{
		return false;
	}
	Params.DiffuseTextureLocation = GetAttributeLocation(Params.DiffuseTextureName);
	Params.HasDiffuseTextureLocation = GetAttributeLocation((std::string("bHas") + Params.DiffuseTextureName));
	Params.DiffuseColourLocation = GetAttributeLocation(Params.DiffuseColourName);
	Params.SpecularStrengthLocation = GetAttributeLocation(Params.SpecularStrengthName);
	Params.ShininessLocation = GetAttributeLocation(Params.ShininessName);

	return true;
}

void CPBRShader::BindShader()
{
	CShader::BindShader();

	// TODO: Improve with light objects
	Lighting::PassLightingToShader();
}

void CPBRShader::UploadMaterialParameters(const ShaderParameters& InParams)
{
	GetGraphicsAPI()->PassAttributeToShader(Params.DiffuseTextureLocation, InParams.DiffuseTexture);
	const bool bHasTexture = InParams.DiffuseTexture != nullptr;
	GetGraphicsAPI()->PassAttributeToShader(Params.HasDiffuseTextureLocation, bHasTexture);
	GetGraphicsAPI()->PassAttributeToShader(Params.DiffuseColourLocation, InParams.DiffuseColour);
	GetGraphicsAPI()->PassAttributeToShader(Params.SpecularStrengthLocation, InParams.SpecularStrength);
	GetGraphicsAPI()->PassAttributeToShader(Params.ShininessLocation, InParams.Shininess);
}

bool CPBRShader::HasTexture(const ShaderParameters& InParams)
{
	return InParams.DiffuseTexture != nullptr;
}

std::ostream& operator<<(std::ostream& os, const CPBRShader::ShaderParameters& InShaderParameters)
{	
	InShaderParameters.SerializeDiffuseColour(os);
	InShaderParameters.SerializeDiffuseTexture(os);
	InShaderParameters.SerializeSpecularStrength(os);
	InShaderParameters.SerializeShininess(os);
	return os;
}

std::istream& operator>>(std::istream& is, CPBRShader::ShaderParameters& InShaderParameters)
{
	InShaderParameters.DeserializeDiffuseColour(is);
	InShaderParameters.DeserializeDiffuseTexture(is);
	InShaderParameters.DeserializeSpecularStrength(is);
	InShaderParameters.DeserializeShininess(is);
	return is;
}
