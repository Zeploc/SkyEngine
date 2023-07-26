// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Platform/PlatformInterface.h"

class WindowsPlatform : public IPlatformInterface
{
public:
	virtual std::string GetPlatformDisplayName();
	
};
