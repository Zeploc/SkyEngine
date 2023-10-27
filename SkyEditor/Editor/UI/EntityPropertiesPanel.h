// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Canvas/UICanvas.h"

class EditorScene;

class CEntityPropertiesPanel : public CUICanvas
{
public:
	CEntityPropertiesPanel(TWeakPointer<CEngineWindow> InOwningWindow);
	virtual void OnRender() override;

	TPointer<EditorScene> EditorScene;
};
