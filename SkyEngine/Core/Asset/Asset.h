// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"
#include "Core/Object.h"

class CAssetObject;

class ENGINE_API CAsset : public std::enable_shared_from_this<CAsset>
{
public:
	CAsset(const std::string& AssetPath, const std::string& InClass = std::string());

	/* Gets object if loaded, otherwise loads it first */
	TAssetObjectPointer<CAssetObject> Load();
	/* Gets object if loaded, otherwise loads it first */
	template<typename T>
	TAssetObjectPointer<T> Load();

	bool IsLoaded() const { return Object.IsValid();/* != nullptr;*/ }
	
	void Unload();
	void Unloaded();
	bool Delete();
	/* Unloads the asset and reloads from disk, returning new object*/
	TAssetObjectPointer<CAssetObject> Reload();
	bool Save();
	void SetDefaultObject(TAssetObjectPointer<CAssetObject> NewObject);
	void Open();
	/* Clears the object link for this asset (Will load from file when load called again) */
	void DisconnectObject();

	std::string GetAbsoluteFilePath() const;

	std::string FilePath;
	std::string DisplayName;
	std::string ClassName;
	TArray<std::string> Metadata;
	
	// TODO: Unsaved
	
	/* Creates the object for this asset when loading based on the class and meta data */
	THardPointer<CAssetObject> MakeObject() const;

protected:
	void ApplyAssetData(std::string AssetData);
	
	// TODO: Only for editor
	/* If wanting to get this externally, use Load() */
	TAssetObjectPointer<CAssetObject> Object = nullptr;
};

template <typename T>
TAssetObjectPointer<T> CAsset::Load()
{
	return Cast<T>(Load());
}
