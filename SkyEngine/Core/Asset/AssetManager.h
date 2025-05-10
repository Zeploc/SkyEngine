// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class CAsset;

class ENGINE_API CAssetManager
{
public:
	static CAssetManager* GetAssetManager();
	TObjectPointer<CAsset> AddAsset(const std::string& AssetPath, const std::string& NewClass = std::string());
	TAssetObjectPointer<CAssetObject> LoadAsset(const std::string& AssetPath);
	
	/* Takes the loaded object that should be unloaded via the asset */
	bool UnloadAsset(TAssetObjectPointer<CAssetObject> AssetObject);
	void UnloadAsset(TWeakPointer<CAsset> Asset);
	
	TObjectPointer<CAsset> FindAsset(const std::string& AssetPath) const;
	bool ContainsScannedAsset(const std::string& AssetPath) const;
	bool DoesAssetExist(const std::string& AssetPath) const;
	bool DeleteAsset(TObjectPointer<CAsset> Asset);

	void SetAssetPath(const std::string& DirectoryPath);
	void ScanForAssets();
	std::string GetAssetPath() const { return AssetDirectory; }
	TArray<TObjectPointer<CAsset>> GetAssets() const;
	TArray<TObjectPointer<CAsset>> GetAssetsOfClass(const std::string& ClassName) const;
	TObjectPointer<CAsset> GetAssetByName(const std::string& AssetName) const;

protected:
	/* Shared ptrs of assets, only given weak ptrs outside to this to maintain control */
	TArray<THardPointer<CAsset>> Assets;
	std::string AssetDirectory;
};
