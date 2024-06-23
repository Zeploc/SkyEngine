// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class CTexture;

class ENGINE_API CTextureManager
{
public:
	void AddLoadedTexture(TPointer<CTexture> NewTexture);
	TPointer<CTexture> AddNewTexture(const std::string& TexturePath);
	TPointer<CTexture> FindTexture(const std::string& TextureSource) const;
	TArray<TPointer<CTexture>> GetAllLoadedTextures() const { return Textures; }
	
protected:
	TArray<TPointer<CTexture>> Textures;
};
