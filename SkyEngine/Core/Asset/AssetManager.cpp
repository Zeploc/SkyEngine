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
	return &GetApplication()->AssetManager;
}

TObjectPointer<CAsset> CAssetManager::AddAsset(const std::string& AssetPath, const std::string& NewClass)
{	
	TObjectPointer<CAsset> FoundAsset = FindAsset(AssetPath);
	if (!FoundAsset)
	{
		THardPointer<CAsset> NewAsset = CreatePointer<CAsset>(StringUtils::NormalizePath(AssetPath), NewClass);
		Assets.push_back(NewAsset);
		FoundAsset = NewAsset;
	}
	return FoundAsset;
}

TAssetObjectPointer<CAssetObject> CAssetManager::LoadAsset(const std::string& AssetPath)
{
	const TObjectPointer<CAsset> LoadedAsset = AddAsset(AssetPath);
	return LoadedAsset->Load();
}

bool CAssetManager::UnloadAsset(TAssetObjectPointer<CAssetObject> AssetObject)
{
	if (!AssetObject)
	{
		return false;
	}
	UnloadAsset(AssetObject->Asset);
	return true;
}

void CAssetManager::UnloadAsset(TWeakPointer<CAsset> Asset)
{
	if (Asset.expired())
	{
		return;
	}
	const std::shared_ptr<CAsset> AssetPtr = Asset.lock();
	// Utils::RemoveFromArray(Assets, AssetPtr);
	AssetPtr->Unloaded();
}

TObjectPointer<CAsset> CAssetManager::FindAsset(const std::string& AssetPath) const
{
	std::string NormalizePath = StringUtils::NormalizePath(AssetPath);
	for (THardPointer<CAsset> Asset : Assets)
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
	return FindAsset(AssetPath).IsValid();// != nullptr;
}

bool CAssetManager::DoesAssetExist(const std::string& AssetPath) const
{
	return CFileManager::FileExists(PathUtils::CombinePath(AssetDirectory, AssetPath));
}

bool CAssetManager::DeleteAsset(TObjectPointer<CAsset> Asset)
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
	const bool bRemovedFromAssets = Utils::RemoveFromArray(Assets, Asset->shared_from_this());
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

TArray<TObjectPointer<CAsset>> CAssetManager::GetAssets() const
{
	return Utils::ArrayConvert<TObjectPointer<CAsset>>(Assets);
}

TArray<TObjectPointer<CAsset>> CAssetManager::GetAssetsOfClass(const std::string& ClassName) const
{
	TArray<TObjectPointer<CAsset>> FoundAssets;
	for (const THardPointer<CAsset>& Asset : Assets)
	{
		if (Asset->ClassName == ClassName)
		{
			FoundAssets.push_back(Asset);
		}
	}
	return FoundAssets;
}

TObjectPointer<CAsset> CAssetManager::GetAssetByName(const std::string& AssetName) const
{
	for (const THardPointer<CAsset>& Asset : Assets)
	{
		if (Asset->DisplayName == AssetName)
		{
			return Asset;
		}
	}
	return nullptr;
}
