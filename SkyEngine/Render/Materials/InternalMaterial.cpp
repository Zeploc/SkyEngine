// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "InternalMaterial.h"

#include "Core/Application.h"
#include "Graphics/Renderer.h"
#include "Render/Shaders/Shader.h"

CMaterialInterface::CMaterialInterface(const std::string& InMaterialName, const std::string& InMaterialPath)
{
	MaterialName = InMaterialName;
	MaterialAssetPath = InMaterialPath;
	
	// TODO: Read from material path
	// Convert to values and store
	// Get shader from material file and store pointer
}

void CMaterialInterface::BindMaterial()
{
	ensure(GetRenderer()->ActiveShader == GetBaseShader(), "Attempted to bind material when shader wasn't active!");
	GetBaseShader()->BindMaterial(shared_from_this());
}