// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"

class CContentBrowser : public CUICanvas
{
public:
	CContentBrowser(const TWeakPointer<CEngineWindow>& InOwningWindow);
	void OnRender() override;

protected:
	std::string Directory = "";
};
