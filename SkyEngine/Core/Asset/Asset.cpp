#include "SEPCH.h"
#include "Asset.h"

#include <fstream>

#include "Core/Asset/AssetObject.h"
#include "Core/StringUtils.h"
#include "Platform/File/FileManager.h"
#include "Platform/File/PathUtils.h"
#include "Render/Materials/Material.h"
#include "Render/Materials/MaterialType.h"
#include "Render/Shaders/PBRShader.h"
#include "Render/Textures/Texture.h"
#include "System/LogManager.h"

CAsset::CAsset(const std::string& AssetPath, const std::string& InClass)
{
	FilePath = AssetPath;
	DisplayName = PathUtils::GetFileName(FilePath, false);

	if (!InClass.empty())
	{
		ClassName = InClass;
	}
	else
	{
		// Read class from file
		std::string FileContents;
		const std::string FullPath = PathUtils::CombinePath(GetAssetManager()->GetAssetPath(), FilePath);
		if (CFileManager::ReadFile(FullPath, FileContents))
		{
			std::stringstream FileStream(FileContents);
			std::string _;
			std::string AssetData;
			std::getline(FileStream, _, '[');
			std::getline(FileStream, AssetData, ']');
			ApplyAssetData(AssetData);
		}
		else
		{
			LOG_WARNING("Failed to find asset at path {}", AssetPath);
		}
	}	
}

void CAsset::ApplyAssetData(std::string AssetData)
{
	TArray<std::string> Tokens = StringUtils::Split(AssetData, ":");
	ClassName = Tokens[0];
	Tokens.erase(Tokens.begin());
	Metadata = Tokens;
}

TAssetObjectPointer<CAssetObject> CAsset::Load()
{
	if (Object)
	{
		// CLogManager::Get()->DisplayWarning(std::format("Attempted to load already loaded asset! Path: {0}", FilePath));
		return Object;
	}
	CLogManager::Get()->DisplayMessage(std::format("Loading asset at {0}", FilePath));
	
	std::string FileContents;
	const std::string FullPath = PathUtils::CombinePath(GetAssetManager()->GetAssetPath(), FilePath);
	if (!CFileManager::ReadFile(FullPath, FileContents))
	{
		return nullptr;
	}

	std::stringstream FileStream(FileContents);
	std::string _;
	std::string AssetData;
	std::getline(FileStream, _, '[');
	std::getline(FileStream, AssetData, ']');
	ApplyAssetData(AssetData);
	if (!ensure(!ClassName.empty(), "Class name must be valid"))
	{
		return nullptr;
	}

	const TSharedPointer<CAssetObject> CreatedObject = MakeObject();
	ensure(CreatedObject != nullptr, "Failed to create class from name [%s]!", ClassName);
	IAssetObjectInterface* AssetInterface = dynamic_cast<IAssetObjectInterface*>(CreatedObject.get());
	FileStream >> AssetInterface;

	SetDefaultObject(CreatedObject);
	AssetInterface->OnLoaded();

	return Object;
}

void CAsset::Unload()
{
	if (!Object)
	{
		return;
	}
	GetAssetManager()->UnloadAsset(Object);
}

void CAsset::Unloaded()
{
	if (!ensure(Object, "Attempted to unload a unloaded asset!"))
	{
		return;
	}
	TSharedPointer<CAssetObject> AssetObject = Object.GetWeak().lock();	
	IAssetObjectInterface* AssetInterface = GetInterface<IAssetObjectInterface>(AssetObject);
	AssetInterface->OnUnloaded();
	AssetInterface->Asset = nullptr;
	DisconnectObject();
}

bool CAsset::Delete()
{
	if (IsLoaded())
	{
		LOG_WARNING("Attempted to delete asset {} while it was still loaded!", DisplayName);
		return false;
	}
	return GetAssetManager()->DeleteAsset(shared_from_this());
}

TAssetObjectPointer<CAssetObject> CAsset::Reload()
{
	// If not loaded, a simple load is enough
	if (!Object)
	{
		return Load();
	}
	
	// Required to stop it being cleaned up
	TSharedPointer<CAsset> Cached = shared_from_this();
	TAssetObjectPointer<CAssetObject> OriginalObject = Object;
	Unload();
	// Store original object to redirect

	// Reload by clearing object so it recreates
	Load();
	
	// TODO: Use proper asset/object management to redirect/reload instead of this garbage
	// Reassign shared ptr memory to new object
	std::shared_ptr<CAssetObject> OriginalPtr = (OriginalObject.GetWeak().lock());
	CAssetObject& AssetObject = *OriginalPtr;
	AssetObject = *Object;
	
	OriginalPtr.reset();
	
	return Object;
}

bool CAsset::Save()
{
	if (!Object)
	{
		return false;
	}
	TSharedPointer<CAssetObject> AssetObject = Object.GetWeak().lock();	
	IAssetObjectInterface* AssetInterface = GetInterface<IAssetObjectInterface>(AssetObject);
	// Update meta data
	Metadata = AssetInterface->GetMetaData();
	
	std::stringstream Serialized;
	const std::string AssetClassName = AssetInterface->GetAssetClassName();
	std::string AssetData = AssetClassName;
	for (const std::string& Data : Metadata)
	{
		AssetData += std::string(":") + Data;
	}
	Serialized << "[" + AssetData + "]\n";
	Serialized << AssetInterface;

	const std::string FullPath = PathUtils::CombinePath(GetAssetManager()->GetAssetPath(), FilePath);
	if (!CFileManager::SaveFile(FullPath, Serialized.str()))
	{
		return false;
	}
	return true;
}

void CAsset::SetDefaultObject(TAssetObjectPointer<CAssetObject> NewObject)
{
	NewObject->Asset = shared_from_this();
	Metadata = NewObject->GetMetaData();
	Object = NewObject;
	HardObject = Object->shared_from_this();
}

void CAsset::Open()
{
	// Confirm loaded first
	Load();
	TSharedPointer<CAssetObject> AssetObject = Object.GetWeak().lock();	
	IAssetObjectInterface* AssetInterface = GetInterface<IAssetObjectInterface>(AssetObject);
	if (AssetInterface)
	{
		AssetInterface->Open();
	}
}

void CAsset::DisconnectObject()
{
	Object = nullptr;
	HardObject = nullptr;
}

std::string CAsset::GetAbsoluteFilePath() const
{
	return PathUtils::CombinePath(GetApplication()->GetContentDirectory(), FilePath);
}

TSharedPointer<CAssetObject> CAsset::MakeObject() const
{
	if (ClassName == CMaterialInterface::GetStaticName())
	{
		ensure(!Metadata.empty(), "Shader meta data required for material");
		if (Metadata.empty())
		{
			return nullptr;
		}
		return CMaterialInterface::MakeMaterialFromShaderName(Metadata[0], DisplayName);
	}
	if (ClassName == Scene::GetStaticName())
	{
		return std::make_shared<Scene>(DisplayName);
	}
	if (ClassName == CTexture::GetStaticName())
	{
		return std::make_shared<CTexture>();
	}
	if (ClassName == CMesh::GetStaticName())
	{
		return std::make_shared<CMesh>();
	}
	return nullptr;
}
