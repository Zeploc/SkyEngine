// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class ENGINE_API CObject : public std::enable_shared_from_this<CObject>
{
public:
	virtual ~CObject() = default;
};
