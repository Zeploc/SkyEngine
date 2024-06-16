#include "SEPCH.h"
#include "AssetManager.h"

#include "Asset.h"
#include "Core/Application.h"
#include "Core/StringUtils.h"
#include "Platform/File/FileManager.h"
#include "Platform/File/PathUtils.h"
#include "System/LogManager.h"
#include "System/Utils.h"

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

bool CAssetManager::ContainsScannedAsset(const std::string& AssetPath) const
{
	return FindAsset(AssetPath) != nullptr;
}

bool CAssetManager::DoesAssetExist(const std::string& AssetPath) const
{
	return CFileManager::FileExists(PathUtils::CombinePath(AssetDirectory, AssetPath));
}

bool CAssetManager::DeleteAsset(TPointer<CAsset> Asset)
{
	if (!Asset)
	{
		return false;
	}
	if (Asset->IsLoaded())
	{
		LOG_WARNING("Attempted to delete asset {} while it was still loaded!", Asset->DisplayName);
		return false;
	}
	const bool bSuccessfulDeletion = CFileManager::Delete_File(PathUtils::CombinePath(AssetDirectory, Asset->FilePath));
	if (!bSuccessfulDeletion)
	{
		LOG_WARNING("Failed to delete asset file {} at {}!", Asset->DisplayName, Asset->FilePath);
		return false;
	}
	const bool bRemovedFromAssets = Utils::RemoveFromArray(Assets, Asset);
	if (!bRemovedFromAssets)
	{
		LOG_WARNING("Failed to remove asset {} from asset manager!", Asset->DisplayName);
	}
	return bRemovedFromAssets;
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
