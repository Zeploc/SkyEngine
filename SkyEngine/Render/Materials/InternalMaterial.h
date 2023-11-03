// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class CShader;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API CMaterialInterface : public std::enable_shared_from_this<CMaterialInterface>
{
public:
	virtual ~CMaterialInterface() = default;
	// TODO: Load asset from path
	CMaterialInterface(const std::string& InMaterialName, const std::string& InMaterialPath = std::string());
	virtual void BindMaterial();

	// TODO: place holder, replace
	virtual bool HasTexture() = 0;

	virtual TPointer<CShader> GetBaseShader() = 0;
	
	// Flags
	bool bTwoSided = false;
	bool bDepthTest = true;
	// TODO: Blend mode (translucent)	
protected:

	std::string MaterialAssetPath; 
	std::string MaterialName; 
};
