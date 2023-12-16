// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class CMaterialInterface;

class ENGINE_API CMaterialManager
{
public:
	void AddMaterial(TPointer<CMaterialInterface> NewMaterial);
	TPointer<CMaterialInterface> FindMaterial(const std::string& MaterialName) const;
	TArray<TPointer<CMaterialInterface>> GetAllLoadedMaterials() const { return Materials; }
	
private:
	TArray<TPointer<CMaterialInterface>> Materials;
};
