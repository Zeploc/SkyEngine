// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "MaterialManager.h"

#include "Material.h"
#include "Core/Application.h"
#include "Core/Asset/Asset.h"

void CMaterialManager::AddMaterial(TAssetObjectPointer<CMaterialInterface> NewMaterial)
{
	Materials.push_back(NewMaterial.GetWeak().lock());
}

TAssetObjectPointer<CMaterialInterface> CMaterialManager::FindMaterial(const std::string& MaterialName) const
{
	for (TSharedPointer<CMaterialInterface> Material : Materials)
	{
		if (!ensure(Material.get(), "Empty Materail in materail manager list!"))
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

TArray<TAssetObjectPointer<CMaterialInterface>> CMaterialManager::GetAllLoadedMaterials() const
{
	return Utils::ArrayConvert<TAssetObjectPointer<CMaterialInterface>>(Materials);
}
