// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Platform/PlatformInterface.h"

class WindowsPlatform : public IPlatformInterface
{
public:
	~WindowsPlatform() override;
	std::string GetPlatformDisplayName() override;
	
	TPointer<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, SVector2 InWindowSize, bool bFullScreen) override;
};
