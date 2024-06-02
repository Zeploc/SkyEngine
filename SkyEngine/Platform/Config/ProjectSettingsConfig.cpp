// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "ProjectSettingsConfig.h"

CProjectSettingsConfig::CProjectSettingsConfig()
	: CConfig(ProjectSettingsPath)
{
	DefineConfigVariable(GraphicsMode);
	DefineConfigVariable(ProjectName);
	DefineConfigVariable(DefaultResolution);
}

TPointer<CProjectSettingsConfig> CProjectSettingsConfig::Get()
{
	return GetConfig<CProjectSettingsConfig>();
}
