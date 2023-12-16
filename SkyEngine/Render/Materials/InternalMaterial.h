// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Object.h"
#include "Core/Asset/AssetInterface.h"

class CShader;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API CMaterialInterface : public CObject, public IAssetObjectInterface//, public std::enable_shared_from_this<CMaterialInterface>
{
public:
	virtual ~CMaterialInterface() = default;
	CMaterialInterface();
	virtual void BindMaterial();
	static std::string GetStaticName();
	static TPointer<CMaterialInterface> MakeMaterialFromClassName(const std::string& ClassName);

	// Override shared_from_this so manual downcast not needed
	// ReSharper disable once CppHidingFunction
	std::shared_ptr<CMaterialInterface> shared_from_this();

	// TODO: place holder, replace
	virtual bool HasTexture() = 0;

	virtual TPointer<CShader> GetBaseShader() = 0;
	virtual std::string GetMaterialName() const = 0;
	
	// Flags
	bool bTwoSided = false;
	bool bDepthTest = true;
	// TODO: Blend mode (translucent)
	
protected:
};
