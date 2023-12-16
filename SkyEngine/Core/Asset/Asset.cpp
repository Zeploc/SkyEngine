#include "SEPCH.h"
#include "Asset.h"

#include <fstream>

#include "Core/Object.h"
#include "Platform/File/FileManager.h"
#include "Platform/File/PathUtils.h"
#include "Render/Materials/Material.h"
#include "Render/Materials/MaterialType.h"
#include "Render/Shaders/PBRShader.h"
#include "System/LogManager.h"

CAsset::CAsset(const std::string& AssetPath)
{
	FilePath = AssetPath;
	DisplayName = PathUtils::GetFileName(FilePath, false);
	
	std::string FileContents;
	const std::string FullPath = PathUtils::CombinePath(GetAssetManager()->GetAssetPath(), FilePath);
	if (CFileManager::ReadFile(FullPath, FileContents))
	{
		std::stringstream FileStream(FileContents);
		std::string _;
		std::getline(FileStream, _, '[');
		std::getline(FileStream, Class, ']');
	}
	else
	{
		CLogManager::Get()->DisplayWarning(std::format("Failed to find asset at path {}", AssetPath));
	}

}

TPointer<CObject> CAsset::Load()
{
	if (Object)
	{
		CLogManager::Get()->DisplayWarning(std::format("Attempted to load already loaded asset! Path: {0}", FilePath));
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
	std::string ClassName;
	std::string _;
	std::getline(FileStream, _, '[');
	std::getline(FileStream, ClassName, ']');

	const TPointer<CObject> CreatedObject = MakeObjectFromClassName(ClassName);
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
	IAssetObjectInterface* AssetInterface = dynamic_cast<IAssetObjectInterface*>(Object.get());
	AssetInterface->OnUnloaded();
	Object.reset();
}

void CAsset::Reload()
{
	Unload();
	Load();
}

bool CAsset::Save()
{
	if (!Object)
	{
		return false;
	}
	IAssetObjectInterface* AssetInterface = dynamic_cast<IAssetObjectInterface*>(Object.get());
	std::stringstream Serialized;
	const std::string AssetClassName = AssetInterface->GetAssetClassName();
	Serialized << "[" + AssetClassName + "]\n";
	Serialized << AssetInterface;

	const std::string FullPath = PathUtils::CombinePath(GetAssetManager()->GetAssetPath(), FilePath);
	if (!CFileManager::SaveFile(FullPath, Serialized.str()))
	{
		return false;
	}
	return true;
}

void CAsset::SetDefaultObject(TPointer<CObject> NewObject)
{
	IAssetObjectInterface* AssetInterface = dynamic_cast<IAssetObjectInterface*>(NewObject.get());
	if (!AssetInterface)
	{
		return;
	}
	AssetInterface->Asset = shared_from_this();
	Object = NewObject;
}

TPointer<CObject> CAsset::MakeObjectFromClassName(const std::string& ClassName)
{
	if (ClassName.starts_with(CMaterialInterface::GetStaticName()))
	{		
		return CMaterialInterface::MakeMaterialFromClassName(ClassName);
	}
	if (ClassName == Scene::GetStaticName())
	{
		return std::make_shared<Scene>("Untitled");
	}
	if (ClassName == "MeshAsset")
	{
		// TODO: Mesh Asset
		return std::make_shared<CMeshData>();
	}
	return nullptr;
}
