// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "PBRShader.h"

#include "Graphics/GraphicsInstance.h"
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

void CPBRShader::BindShader(const TPointer<IGraphicsInstance>& InGraphicsInterface)
{
	CShader::BindShader(InGraphicsInterface);

	// TODO: Improve with light objects
	Lighting::PassLightingToShader(InGraphicsInterface);
}

void CPBRShader::UploadMaterialParameters(const TPointer<IGraphicsInstance>& InGraphicsInstance, const ShaderParameters& InParams)
{
	InGraphicsInstance->PassAttributeToShader(Params.DiffuseTextureLocation, InParams.DiffuseTexture);
	const bool bHasTexture = InParams.DiffuseTexture != nullptr;
	InGraphicsInstance->PassAttributeToShader(Params.HasDiffuseTextureLocation, bHasTexture);
	InGraphicsInstance->PassAttributeToShader(Params.DiffuseColourLocation, InParams.DiffuseColour);
	InGraphicsInstance->PassAttributeToShader(Params.SpecularStrengthLocation, InParams.SpecularStrength);
	InGraphicsInstance->PassAttributeToShader(Params.ShininessLocation, InParams.Shininess);
}

bool CPBRShader::HasTexture(const ShaderParameters& InParams)
{
	return InParams.DiffuseTexture != nullptr;
}
