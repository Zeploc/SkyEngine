// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "TextureUtils.h"

#include "Texture.h"
#include "Core/Application.h"
#include "Core/Asset/Asset.h"
#include "Platform/File/PathUtils.h"
#include "System/LogManager.h"

TAssetObjectPointer<CTexture> CTextureUtils::AddNewTexture(const std::string& TexturePath)
{
	TSharedPointer<CTexture> NewTexture = CreatePointer<CTexture>(TexturePath);
	if (!GetGraphicsAPI()->GenerateTexture(NewTexture))
	{
		NewTexture = nullptr;
		return nullptr;
	}
	CLogManager::Get()->DisplayMessage("Adding Texture, \"" + std::string(TexturePath) + "\"");
	std::string TextureAssetName = PathUtils::GetFileName(TexturePath, false);
	const TObjectPointer<CAsset> TextureAsset = GetAssetManager()->AddAsset("Textures/" + TextureAssetName + ".sasset", CTexture::GetStaticName());
	TextureAsset->SetDefaultObject(NewTexture);
	return NewTexture;
}

TAssetObjectPointer<CTexture> CTextureUtils::FindTextureBySource(const std::string& TextureSourcePath)
{
	TArray<TObjectPointer<CAsset>> TextureAssets = GetAssetManager()->GetAssetsOfClass(CTexture::GetStaticName());
	for (const TObjectPointer<CAsset>& Asset : TextureAssets)
	{
		// Force loads all textures - ideally would be asset registry searchable path
		TAssetObjectPointer<CTexture> Texture = Asset->Load<CTexture>();
		if (!Texture)
		{
			continue;
		}
		if (PathUtils::GetNormalizePath(Texture->Path) == PathUtils::GetNormalizePath(TextureSourcePath))
		{
			return Texture;
		}
	}
	// Fall back to trying to find asset path matching texture source - Only relevant if this function is misused
	if (const TObjectPointer<CAsset> Asset = GetAssetManager()->GetAssetByName(TextureSourcePath))
	{
		if (TAssetObjectPointer<CTexture> LoadedTexture = Asset->Load<CTexture>())
		{
			return LoadedTexture;
		}
	}

	return nullptr;
}
