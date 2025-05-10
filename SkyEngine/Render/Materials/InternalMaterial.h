// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"

class CShader;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API CMaterialInterface : public CAssetObject
{
public:
	virtual ~CMaterialInterface() override = default;
	CMaterialInterface();
	virtual void BindMaterial();
	static std::string GetStaticName();
	static THardPointer<CMaterialInterface> MakeMaterialFromShaderName(const std::string& ShaderName, const std::string& MaterialName);

	// Override shared_from_this so manual downcast not needed
	// ReSharper disable once CppHidingFunction
	std::shared_ptr<CMaterialInterface> shared_from_this();

	// TODO: place holder, replace
	virtual bool HasTexture() = 0;
	virtual TArray<struct SShaderParameter>& GetMaterialVariables() = 0;

	virtual THardPointer<CShader> GetBaseShader() = 0;
	virtual std::string GetMaterialName() const = 0;
	void Open() override;

	// Flags
	bool bTwoSided = false;
	bool bDepthTest = true;
	// TODO: Blend mode (translucent)
	
protected:
};
