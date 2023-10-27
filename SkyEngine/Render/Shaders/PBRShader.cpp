// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "PBRShader.h"

#include "Core/Application.h"
#include "Render/Lighting.h"

CPBRShader::CPBRShader()
: CShader("PBR","Resources/Shaders/PBRVertexShader.vs", "Resources/Shaders/PBRFragmentShader.fs")
{
}

CPBRShader::~CPBRShader()
{
}

bool CPBRShader::CompileShader()
{
	if (!CShader::CompileShader())
	{
		return false;
	}
	// TODO: Replace with graphics api 
	Params.DiffuseTextureLocation = glGetUniformLocation(ShaderProgram, Params.DiffuseTextureName.c_str());
	Params.HasDiffuseTextureLocation = glGetUniformLocation(ShaderProgram, (std::string("bHas") + Params.DiffuseTextureName).c_str());
	Params.DiffuseColourLocation = glGetUniformLocation(ShaderProgram, Params.DiffuseColourName.c_str());
	Params.SpecularStrengthLocation = glGetUniformLocation(ShaderProgram, Params.SpecularStrengthName.c_str());
	Params.ShininessLocation = glGetUniformLocation(ShaderProgram, Params.ShininessName.c_str());

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
