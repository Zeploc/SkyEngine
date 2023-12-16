#include "SEPCH.h"
#include "MaterialManager.h"

#include "Material.h"
#include "Core/Application.h"
#include "Core/Asset/Asset.h"

void CMaterialManager::AddMaterial(TPointer<CMaterialInterface> NewMaterial)
{
	Materials.push_back(NewMaterial);
}

TPointer<CMaterialInterface> CMaterialManager::FindMaterial(const std::string& MaterialName) const
{
	for (TPointer<CMaterialInterface> Material : Materials)
	{
		if (Material->GetMaterialName() == MaterialName)
		{
			return Material;
		}
	}
	if (const TPointer<CAsset> Asset = GetAssetManager()->GetAssetByName(MaterialName))
	{
		if (TPointer<CMaterialInterface> LoadedMaterial = Asset->Load<CMaterialInterface>())
		{
			return LoadedMaterial;
		}
	}

	return nullptr;
}
