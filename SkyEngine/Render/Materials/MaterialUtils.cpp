// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "MaterialUtils.h"

#include "Material.h"
#include "Core/Application.h"
#include "Core/Asset/Asset.h"

bool CMaterialUtils::RegisterNewMaterial(TAssetObjectPointer<CMaterialInterface> NewMaterial)
{
	if (!ensure(NewMaterial, "Attempted to register null material"))
	{
		return false;
	}
	TObjectPointer<CAsset> MaterialAsset = GetAssetManager()->AddAsset(std::string("Materials/") + NewMaterial->GetMaterialName() + std::string(".sasset"), NewMaterial->GetAssetClassName());
	if (!MaterialAsset)
	{
		return false;
	}
	MaterialAsset->SetDefaultObject(NewMaterial);
	return MaterialAsset->Save();
}

TAssetObjectPointer<CMaterialInterface> CMaterialUtils::FindMaterial(const std::string& MaterialName)
{
	TArray<TObjectPointer<CAsset>> MaterialAssets = GetAssetManager()->GetAssetsOfClass(CMaterialInterface::GetStaticName());
	for (const TObjectPointer<CAsset>& Asset : MaterialAssets)
	{
		// Force loads all textures - ideally would be asset registry searchable path
		TAssetObjectPointer<CMaterialInterface> Material = Asset->Load<CMaterialInterface>();
		if (!Material)
		{
			continue;
		}
		if (Material->GetMaterialName() == MaterialName)
		{
			return Material;
		}
	}
	if (const TObjectPointer<CAsset> Asset = GetAssetManager()->GetAssetByName(MaterialName))
	{
		if (TAssetObjectPointer<CMaterialInterface> LoadedMaterial = Asset->Load<CMaterialInterface>())
		{
			return LoadedMaterial;
		}
	}

	return nullptr;
}
