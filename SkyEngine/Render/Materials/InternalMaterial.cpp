// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "InternalMaterial.h"

#include "Material.h"
#include "Core/Application.h"
#include "Core/StringUtils.h"
#include "Render/Renderer.h"
#include "Render/Shaders/PBRShader.h"
#include "Render/Shaders/Shader.h"
#include "Render/Shaders/UnlitShader.h"

CMaterialInterface::CMaterialInterface()
{	
}

void CMaterialInterface::BindMaterial()
{
	ensure(GetRenderer()->ActiveShader == GetBaseShader(), "Attempted to bind material when shader wasn't active!");
	GetBaseShader()->BindMaterial(shared_from_this());
}

std::string CMaterialInterface::GetStaticName()
{
	return "Material";
}

TPointer<CMaterialInterface> CMaterialInterface::MakeMaterialFromShaderName(const std::string& ShaderName)
{
	if (ShaderName == CPBRShader::GetStaticName())
	{
		return std::make_shared<CMaterial_PBR>("New Material");
	}
	if (ShaderName == CUnlitShader::GetStaticName())
	{
		return std::make_shared<CMaterial_Unlit>("New Material");
	}
	return nullptr;
}

std::shared_ptr<CMaterialInterface> CMaterialInterface::shared_from_this()
{
	return std::static_pointer_cast<CMaterialInterface>(CAssetObject::shared_from_this());
}

void CMaterialInterface::Open()
{
	// TODO: Material window
}
