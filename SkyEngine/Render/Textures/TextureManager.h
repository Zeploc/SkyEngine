// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Object.h"

class CTexture;

class ENGINE_API CTextureManager
{
public:
	void AddLoadedTexture(TAssetObjectPointer<CTexture> NewTexture);
	TAssetObjectPointer<CTexture> AddNewTexture(const std::string& TexturePath);
	TAssetObjectPointer<CTexture> FindTexture(const std::string& TextureSource) const;
	TArray<TAssetObjectPointer<CTexture>> GetAllLoadedTextures() const { return Textures; }
	
protected:
	TArray<TAssetObjectPointer<CTexture>> Textures;
};
