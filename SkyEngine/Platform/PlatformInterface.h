// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

#include <string>

#include "Math/Internal/Vector2.decl.h"

class IGraphicsWindow;

class ENGINE_API IPlatformInterface
{
public:
	virtual ~IPlatformInterface() = default;
	
	virtual TPointer<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, SVector2 InWindowSize, bool bFullScreen) = 0;

	virtual std::string GetPlatformDisplayName() = 0;
};
