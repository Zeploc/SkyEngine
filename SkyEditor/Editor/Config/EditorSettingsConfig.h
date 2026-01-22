// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Platform/Config/Config.h"

class CEditorSettingsConfig final : public CConfig
{
public:
	CEditorSettingsConfig();

	static TSharedPointer<CEditorSettingsConfig> Get();

	int GraphicsMode = 0;
	SVector2 Resolution = SVector2(1920, 1080);
	
protected:
	inline static std::string EditorSettingsName = "EditorSettings";

};
