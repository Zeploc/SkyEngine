// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"
#include "Core/Object.h"

class CMaterialInterface;

class CMaterialEditorPanel : public CUICanvas
{
public:
	CMaterialEditorPanel(TWeakPointer<CEngineWindow> InOwningWindow);
	virtual void OnRender() override;

	void SetMaterial(TAssetObjectPointer<CMaterialInterface> InMaterial);

protected:
	TAssetObjectPointer<CMaterialInterface> Material;
	
};
