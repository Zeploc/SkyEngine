// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class CAsset;

class ENGINE_API IAssetObjectInterface
{
public:
	virtual ~IAssetObjectInterface() = default;
	friend std::ostream& operator<<(std::ostream& os, IAssetObjectInterface* InAsset);
	friend std::istream& operator>>(std::istream& is, IAssetObjectInterface* InAsset);
	
	virtual void Serialize(std::ostream& os) = 0;
	virtual void Deserialize(std::istream& is) = 0;
	virtual std::string GetAssetClassName() = 0;

	virtual void OnLoaded() {}
	virtual void OnUnloaded() {}
	
	TPointer<CAsset> Asset;
};
