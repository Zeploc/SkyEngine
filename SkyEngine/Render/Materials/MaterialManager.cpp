#include "SEPCH.h"
#include "MaterialManager.h"

#include "InternalMaterial.h"

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
	return nullptr;
}
