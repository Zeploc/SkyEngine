// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

class CObject;

class ENGINE_API CAsset : public std::enable_shared_from_this<CAsset>
{
public:
	CAsset(const std::string& AssetPath, const std::string& InClass = std::string());

	/* Gets object if loaded, otherwise loads it first */
	TPointer<CObject> Load();
	/* Gets object if loaded, otherwise loads it first */
	template<typename T>
	TPointer<T> Load();

	bool IsLoaded() const { return Object != nullptr; }
	
	void Unload();
	/* Unloads the asset and reloads from disk, returning new object*/
	TPointer<CObject> Reload();
	bool Save();
	void SetDefaultObject(TPointer<CObject> NewObject);
	void Open();
	/* Clears the object link for this asset (Will load from file when load called again) */
	void DisconnectObject();

	std::string FilePath;
	std::string DisplayName;
	std::string ClassName;
	TArray<std::string> Metadata;
	
	// TODO: Unsaved
	

protected:
	/* Creates the object for this asset when loading based on the class and meta data */
	TPointer<CObject> MakeObject() const;
	void ApplyAssetData(std::string AssetData);
	
	// TODO: Only for editor
	TPointer<CObject> Object = nullptr;
};

template <typename T>
TPointer<T> CAsset::Load()
{
	return std::dynamic_pointer_cast<T>(Load());
}
