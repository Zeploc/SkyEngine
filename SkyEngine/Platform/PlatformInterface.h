// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

#include "Math/Internal/Vector2.decl.h"

class CEngineWindow;

class ENGINE_API IPlatformInterface
{
public:
	virtual ~IPlatformInterface() = default;
	
	virtual THardPointer<CEngineWindow> CreateNewWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bFullScreen) = 0;

	/* Returns int for response type */
	virtual int32_t DisplayMessageBox(std::string Title, std::string Message, uint32_t Options) = 0; 

	// Get current system time using the highest resolution time source available on the platform
	virtual double GetTime() = 0;

	virtual std::string GetPlatformDisplayName() = 0;
};
