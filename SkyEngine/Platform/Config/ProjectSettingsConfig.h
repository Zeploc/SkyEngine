// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Config.h"
#include "Math/Internal/Vector2.decl.h"

class ENGINE_API CProjectSettingsConfig final : public CConfig
{
public:
	CProjectSettingsConfig();

	static TPointer<CProjectSettingsConfig> Get();
	
	DeclareConfigInteger(GraphicsMode, 0)
	DeclareConfigString(ProjectName, "Sandbox_Project")
	DeclareConfigVector2(DefaultResolution, SVector2(1920, 1080))
	
protected:
	inline static std::string ProjectSettingsPath = "ProjectSettings";

};
