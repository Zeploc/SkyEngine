// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "InternalMaterial.h"

#include "Graphics/GraphicsInstance.h"
#include "Render/Shaders/Shader.h"

CMaterialInterface::CMaterialInterface(const std::string& InMaterialName, const std::string& InMaterialPath)
{
	MaterialName = InMaterialName;
	MaterialAssetPath = InMaterialPath;
	
	// TODO: Read from material path
	// Convert to values and store
	// Get shader from material file and store pointer
}

void CMaterialInterface::BindMaterial(const TPointer<IGraphicsInstance> InGraphicsInterface)
{
	ensure(InGraphicsInterface->ActiveShader == GetBaseShader(), "Attempted to bind material when shader wasn't active!");
	GetBaseShader()->BindMaterial(InGraphicsInterface, shared_from_this());
}