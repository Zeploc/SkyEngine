// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "EditorSettingsConfig.h"

CEditorSettingsConfig::CEditorSettingsConfig()
	: CConfig(EditorSettingsPath)
{
	DefineConfigVariable(GraphicsMode);
	DefineConfigVariable(Resolution);
}

TPointer<CEditorSettingsConfig> CEditorSettingsConfig::Get()
{
	return GetConfig<CEditorSettingsConfig>();
}
