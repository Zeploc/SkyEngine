// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Platform/PlatformInterface.h"

class WindowsPlatform : public IPlatformInterface
{
public:
	~WindowsPlatform() override;
	std::string GetPlatformDisplayName() override;
	
	TPointer<CEngineWindow> CreateNewWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bFullScreen) override;
	double GetTime() override;
};
