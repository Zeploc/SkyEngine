#include "SEPCH.h"
#include "Asset.h"

#include <fstream>

#include "Core/Object.h"
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

	const THardPointer<CAssetObject> CreatedObject = MakeObject();
	ensure(CreatedObject != nullptr, "Failed to create class from name!");
	IAssetObjectInterface* AssetInterface = dynamic_cast<IAssetObjectInterface*>(CreatedObject.get());
	FileStream >> AssetInterface;
	AssetInterface->Asset = shared_from_this();

	Object = CreatedObject;
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
	THardPointer<CAssetObject> AssetObject = Object.GetWeak().lock();	
	IAssetObjectInterface* AssetInterface = GetInterface<IAssetObjectInterface>(AssetObject);
	AssetInterface->OnUnloaded();
	AssetInterface->Asset = nullptr;
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
	Unload();
	return Load();
}

bool CAsset::Save()
{
	if (!Object)
	{
		return false;
	}
	THardPointer<CAssetObject> AssetObject = Object.GetWeak().lock();	
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
}

void CAsset::Open()
{
	// Confirm loaded first
	Load();
	THardPointer<CAssetObject> AssetObject = Object.GetWeak().lock();	
	IAssetObjectInterface* AssetInterface = GetInterface<IAssetObjectInterface>(AssetObject);
	if (AssetInterface)
	{
		AssetInterface->Open();
	}
}

void CAsset::DisconnectObject()
{
	Object = nullptr;
}

std::string CAsset::GetAbsoluteFilePath() const
{
	return PathUtils::CombinePath(GetApplication()->GetContentDirectory(), FilePath);
}

THardPointer<CAssetObject> CAsset::MakeObject() const
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
	if (ClassName == "MeshAsset")
	{
		// TODO: Mesh Asset
		return std::make_shared<CMeshData>();
	}
	return nullptr;
}
