// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"
#include "Core/Asset/AssetInterface.h"

class ENGINE_API CTexture : public CAssetObject
{
public:
	std::string Path;
	int Width = 0;
	int Height = 0;
	uint32_t TextureID = 0;
	bool bBound = false;

	// TODO:
	bool bAntiAliasing = true;

	CTexture() {}
	CTexture(const std::string InPath);

	bool IsValid() const;

	bool IsBound() const;

	void Bind(const uint32_t InTextureID);

	uint32_t GetTextureRenderID() const
	{
		return TextureID;
	}
	
	// Override shared_from_this so manual downcast not needed
	// ReSharper disable once CppHidingFunction
	std::shared_ptr<CTexture> shared_from_this();
	TWeakPointer<CTexture> weak_from_this();
	
	void OnLoaded() override;

	friend std::ostream& operator<<(std::ostream& os, const THardPointer<CTexture>& InTexture)
	{
		os << InTexture->Path;
		return os;
	}
	friend std::istream& operator>>(std::istream& is, THardPointer<CTexture>& OutTexture)
	{
		is >> OutTexture->Path;
		return is;
	}

	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
	std::string GetAssetClassName() override;
	static std::string GetStaticName();
	void Open() override;

protected:
	
};
