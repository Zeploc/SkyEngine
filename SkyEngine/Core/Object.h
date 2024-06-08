// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Asset/AssetInterface.h"
#include "Core/Core.h"

class ENGINE_API CObject : public std::enable_shared_from_this<CObject>
{
public:
};

class ENGINE_API CAssetObject : public CObject, public IAssetObjectInterface
{
public:
	
	std::shared_ptr<CAssetObject> shared_from_this();
};

