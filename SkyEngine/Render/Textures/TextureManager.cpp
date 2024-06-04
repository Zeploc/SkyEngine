// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "TextureManager.h"

#include "Texture.h"
#include "Core/Application.h"
#include "Core/Asset/Asset.h"
#include "Platform/File/PathUtils.h"
#include "System/LogManager.h"

void CTextureManager::AddLoadedTexture(TPointer<CTexture> NewTexture)
{
	Textures.push_back(NewTexture);
}

TPointer<CTexture> CTextureManager::AddNewTexture(const std::string& TexturePath)
{
	TPointer<CTexture> NewTexture = CreatePointer<CTexture>(TexturePath);
	if (!GetGraphicsAPI()->GenerateTexture(NewTexture))
	{
		NewTexture = nullptr;
		return nullptr;
	}
	CLogManager::Get()->DisplayMessage("Adding Texture, \"" + std::string(TexturePath) + "\", Total Texture Count : " + std::to_string(Textures.size()));
	std::string TextureAssetName = PathUtils::GetFileName(TexturePath, false);
	const TPointer<CAsset> TextureAsset = GetAssetManager()->AddAsset("Textures/" + TextureAssetName + ".sasset", CTexture::GetStaticName());
	TextureAsset->SetDefaultObject(NewTexture);
	AddLoadedTexture(NewTexture);
	return NewTexture;
}

TPointer<CTexture> CTextureManager::FindTexture(const std::string& TextureSource) const
{
	for (TPointer<CTexture> Texture : Textures)
	{
		if (PathUtils::GetNormalizePath(Texture->Path) == PathUtils::GetNormalizePath(TextureSource))
		{
			return Texture;
		}
	}
	if (const TPointer<CAsset> Asset = GetAssetManager()->GetAssetByName(TextureSource))
	{
		if (TPointer<CTexture> LoadedTexture = Asset->Load<CTexture>())
		{
			return LoadedTexture;
		}
	}

	return nullptr;
}
