// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Platform/PlatformInterface.h"

class WindowsPlatform : public IPlatformInterface
{
public:
	std::string GetPlatformDisplayName() override;
	
	Pointer<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen) override;
};
