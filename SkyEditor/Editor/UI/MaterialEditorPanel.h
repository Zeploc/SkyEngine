// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"

class CMaterialInterface;

class CMaterialEditorPanel : public CUICanvas
{
public:
	CMaterialEditorPanel(TWeakPointer<CEngineWindow> InOwningWindow);
	virtual void OnRender() override;

	void SetMaterial(TPointer<CMaterialInterface> InMaterial);

protected:
	TPointer<CMaterialInterface> Material;
	
};
