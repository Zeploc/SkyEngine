// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UnlitShader.h"

#include <glew/glew.h>

#include "Graphics/GraphicsInstance.h"

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
	// TODO: Replace with graphics api
	Params.DiffuseTextureLocation = glGetUniformLocation(ShaderProgram, Params.DiffuseTextureName.c_str());
	Params.HasDiffuseTextureLocation = glGetUniformLocation(ShaderProgram, (std::string("bHas") + Params.DiffuseTextureName).c_str());
	Params.DiffuseColourLocation = glGetUniformLocation(ShaderProgram, Params.DiffuseColourName.c_str());

	return true;
}

void CUnlitShader::BindShader(const TPointer<IGraphicsInstance>& InGraphicsInterface)
{
	CShader::BindShader(InGraphicsInterface);
}

void CUnlitShader::UploadMaterialParameters(const TPointer<IGraphicsInstance>& InGraphicsInstance, const ShaderParameters& InParams)
{
	InGraphicsInstance->PassAttributeToShader(Params.DiffuseTextureLocation, InParams.DiffuseTexture);
	const bool bHasTexture = InParams.DiffuseTexture != nullptr;
	InGraphicsInstance->PassAttributeToShader(Params.HasDiffuseTextureLocation, bHasTexture);
	InGraphicsInstance->PassAttributeToShader(Params.DiffuseColourLocation, InParams.DiffuseColour);
}

bool CUnlitShader::HasTexture(const ShaderParameters& InParams)
{
	return InParams.DiffuseTexture != nullptr;
}
