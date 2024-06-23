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
protected:
	
	TPointer<CConfig> Config;
};
