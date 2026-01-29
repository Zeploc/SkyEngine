// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class CMaterialInterface;

class ENGINE_API CMaterialUtils
{
public:
	/* If a new material was manually made, register it with the asset registry */
	static bool RegisterNewMaterial(TAssetObjectPointer<CMaterialInterface> NewMaterial);
	
	/* Find a material object based on the material name */
	static TAssetObjectPointer<CMaterialInterface> FindMaterial(const std::string& MaterialName);
};
