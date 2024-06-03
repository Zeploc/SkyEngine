// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"

class CConfig;
struct SSerializableVariable;

class CConfigSettingsPanel : public CUICanvas
{
public:
	CConfigSettingsPanel(TWeakPointer<CEngineWindow> InOwningWindow, TPointer<CConfig> InConfig);
	virtual void OnRender() override;

	static void RenderVariableField(SSerializableVariable ConfigVariable);
protected:
	
	TPointer<CConfig> Config;
};
