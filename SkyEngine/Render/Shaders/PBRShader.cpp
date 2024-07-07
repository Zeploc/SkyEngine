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

void CPBRShader::BindShader()
{
	CShader::BindShader();

	// TODO: Improve with light objects
	Lighting::PassLightingToShader();
}

bool CPBRShader::HasTexture(const ShaderParameters& InMaterialParams)
{
	return InMaterialParams.DiffuseTexture.IsValid();// todo: // != nullptr;
}

