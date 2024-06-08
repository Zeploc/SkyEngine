#include "SEPCH.h"
#include "AssetManager.h"

#include "Asset.h"
#include "Core/Application.h"
#include "Core/StringUtils.h"
#include "Platform/File/FileManager.h"

CAssetManager* CAssetManager::GetAssetManager()
{
	return GetAssetManager();
}

TPointer<CAsset> CAssetManager::AddAsset(const std::string& AssetPath, const std::string& NewClass)
{	
	TPointer<CAsset> NewAsset = FindAsset(AssetPath);
	if (!NewAsset)
	{
		NewAsset = CreatePointer<CAsset>(StringUtils::NormalizePath(AssetPath), NewClass);
		Assets.push_back(NewAsset);
	}
	return NewAsset;
}

TPointer<CAssetObject> CAssetManager::LoadAsset(const std::string& AssetPath)
{
	const TPointer<CAsset> LoadedAsset = AddAsset(AssetPath);
	return LoadedAsset->Load();
}

TPointer<CAsset> CAssetManager::FindAsset(const std::string& AssetPath) const
{
	std::string NormalizePath = StringUtils::NormalizePath(AssetPath);
	for (TPointer<CAsset> Asset : Assets)
	{
		if (Asset->FilePath == NormalizePath)
		{
			return Asset;
		}
	}
	return nullptr;
}

bool CAssetManager::ContainsAsset(const std::string& AssetPath) const
{
	return FindAsset(AssetPath) != nullptr;
}

void CAssetManager::SetAssetPath(const std::string& DirectoryPath)
{
	AssetDirectory = DirectoryPath;
}

void CAssetManager::ScanForAssets()
{
	const TArray<std::string> FilePaths = CFileManager::ListFilesInDirectory(AssetDirectory, true);
	for (std::string FilePath : FilePaths)
	{
		StringUtils::Replace(FilePath, AssetDirectory, "");
		AddAsset(FilePath);
	}
}

TArray<TPointer<CAsset>> CAssetManager::GetAssetsOfClass(const std::string& ClassName) const
{
	TArray<TPointer<CAsset>> FoundAssets;
	for (const TPointer<CAsset>& Asset : Assets)
	{
		if (Asset->ClassName == ClassName)
		{
			FoundAssets.push_back(Asset);
		}
	}
	return FoundAssets;
}

TPointer<CAsset> CAssetManager::GetAssetByName(const std::string& AssetName) const
{
	for (const TPointer<CAsset>& Asset : Assets)
	{
		if (Asset->DisplayName == AssetName)
		{
			return Asset;
		}
	}
	return nullptr;
}
