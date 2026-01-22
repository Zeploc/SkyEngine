// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class CMaterialInterface;

class ENGINE_API CMaterialManager
{
public:
	void AddMaterial(TAssetObjectPointer<CMaterialInterface> NewMaterial);
	TAssetObjectPointer<CMaterialInterface> FindMaterial(const std::string& MaterialName) const;
	TArray<TAssetObjectPointer<CMaterialInterface>> GetAllLoadedMaterials() const;
	
private:
	TArray<TSharedPointer<CMaterialInterface>> Materials;
};
