// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Config.h"
#include "Math/Internal/Vector2.decl.h"

class ENGINE_API CProjectSettingsConfig final : public CConfig
{
public:
	CProjectSettingsConfig();

	static THardPointer<CProjectSettingsConfig> Get();

	int GraphicsMode = 0;
	std::string ProjectName = "Sandbox_Project";
	SVector2 DefaultResolution =  SVector2(1920, 1080);
	
protected:
	inline static std::string ProjectSettingsName = "ProjectSettings";

};
