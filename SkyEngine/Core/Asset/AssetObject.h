// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "AssetInterface.h"
#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"
#include "Core/SerializableVariable.h"

class ENGINE_API CAssetObject : public CObject, public IAssetObjectInterface
{
public:
	
	std::shared_ptr<CAssetObject> shared_from_this();
	// TWeakPointer<CAssetObject> weak_from_this();

	TArray<SSerializableVariable> SerializeVariables;
	
	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
};
