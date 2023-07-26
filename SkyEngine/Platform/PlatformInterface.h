// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

#include <string>

class IGraphicsWindow;

class ENGINE_API IPlatformInterface
{
public:
	virtual ~IPlatformInterface() = default;

	virtual std::string GetPlatformDisplayName() = 0;
};
