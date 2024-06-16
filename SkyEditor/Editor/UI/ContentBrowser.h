// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"
#include "Core/Asset/Asset.h"

class CContentBrowser : public CUICanvas
{
public:
	CContentBrowser(const TWeakPointer<CEngineWindow>& InOwningWindow);
	void DeletePopup(TPointer<CAsset> Asset);
	void OnRender() override;

protected:
	std::string Directory = "";
};
