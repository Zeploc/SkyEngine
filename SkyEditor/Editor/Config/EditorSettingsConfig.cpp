// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "EditorSettingsConfig.h"

CEditorSettingsConfig::CEditorSettingsConfig()
	: CConfig(EditorSettingsName)
{
	DefineConfigVariable(GraphicsMode);
	DefineConfigVariable(Resolution);
}

TSharedPointer<CEditorSettingsConfig> CEditorSettingsConfig::Get()
{
	return GetConfig<CEditorSettingsConfig>();
}
