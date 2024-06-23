// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"
#include "Core/Object.h"

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class CAsset;

class ENGINE_API CAssetManager
{
public:
	static CAssetManager* GetAssetManager();
	TPointer<CAsset> AddAsset(const std::string& AssetPath, const std::string& NewClass = std::string());
	TPointer<CAssetObject> LoadAsset(const std::string& AssetPath);
	TPointer<CAsset> FindAsset(const std::string& AssetPath) const;
	bool ContainsScannedAsset(const std::string& AssetPath) const;
	bool DoesAssetExist(const std::string& AssetPath) const;
	bool DeleteAsset(TPointer<CAsset> Asset);

	void SetAssetPath(const std::string& DirectoryPath);
	void ScanForAssets();
	std::string GetAssetPath() const { return AssetDirectory; }
	const TArray<TPointer<CAsset>>& GetAssets() const { return Assets; }
	TArray<TPointer<CAsset>> GetAssetsOfClass(const std::string& ClassName) const;
	TPointer<CAsset> GetAssetByName(const std::string& AssetName) const;

protected:
	TArray<TPointer<CAsset>> Assets;
	std::string AssetDirectory;
};
