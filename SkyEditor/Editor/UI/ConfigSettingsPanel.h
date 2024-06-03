// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"

class CConfig;
struct SConfigVariable;

class CConfigSettingsPanel : public CUICanvas
{
public:
	CConfigSettingsPanel(TWeakPointer<CEngineWindow> InOwningWindow, TPointer<CConfig> InConfig);
	virtual void OnRender() override;

protected:
	void RenderVariableField(SConfigVariable ConfigVariable);
	
	TPointer<CConfig> Config;
};
