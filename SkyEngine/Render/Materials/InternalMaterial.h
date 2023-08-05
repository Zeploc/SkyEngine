// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <memory>
#include <string>

#include "Core/Core.h"

class CShader;
class IGraphicsInstance;

class ENGINE_API CMaterialInterface : public std::enable_shared_from_this<CMaterialInterface>
{
public:
	virtual ~CMaterialInterface() = default;
	CMaterialInterface();
	// TODO: Load asset from path
	CMaterialInterface(const std::string& InMaterialPath);
	virtual void BindMaterial(TPointer<IGraphicsInstance> InGraphicsInterface);

	// TODO: place holder, replace
	virtual bool HasTexture() = 0;

	virtual TPointer<CShader> GetBaseShader() = 0;
	
	// Flags
	bool bTwoSided = false;
	bool bDepthTest = true;
	// TODO: Blend mode (translucent)	
protected:

	std::string MaterialAssetPath; 
};
