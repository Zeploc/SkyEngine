// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "ProjectSettingsConfig.h"

CProjectSettingsConfig::CProjectSettingsConfig()
	: CConfig(ProjectSettingsName)
{
	DefineConfigVariable(GraphicsMode);
	DefineConfigVariable(ProjectName);
	DefineConfigVariable(DefaultResolution);
}

THardPointer<CProjectSettingsConfig> CProjectSettingsConfig::Get()
{
	return GetConfig<CProjectSettingsConfig>();
}
