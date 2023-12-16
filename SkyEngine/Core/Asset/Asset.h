// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

class CObject;

class ENGINE_API CAsset : public std::enable_shared_from_this<CAsset>
{
public:
	CAsset(const std::string& AssetPath);

	TPointer<CObject> Load();
	void Unload();
	void Reload();
	bool Save();
	void SetDefaultObject(TPointer<CObject> NewObject);

	static TPointer<CObject> MakeObjectFromClassName(const std::string& ClassName);

	std::string FilePath;
	std::string DisplayName;
	std::string Class;

	// TODO: Unsaved
	
	template<typename T>
	TPointer<T> Load();

protected:
	// TODO: Only for editor
	TPointer<CObject> Object = nullptr;
};

template <typename T>
TPointer<T> CAsset::Load()
{
	return std::dynamic_pointer_cast<T>(Load());
}
