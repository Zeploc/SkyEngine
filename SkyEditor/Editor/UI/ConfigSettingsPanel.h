// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"

class CConfig;
struct SSerializableVariable;

class CConfigSettingsPanel : public CUICanvas
{
public:
	CConfigSettingsPanel(TWeakPointer<CEngineWindow> InOwningWindow, THardPointer<CConfig> InConfig);
	virtual void OnRender() override;
protected:
	
	THardPointer<CConfig> Config;
};
