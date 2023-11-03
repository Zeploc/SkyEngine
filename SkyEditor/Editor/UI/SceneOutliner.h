// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"

class EditorScene;

class CSceneOutliner : public CUICanvas
{
public:
	CSceneOutliner(TWeakPointer<CEngineWindow> InOwningWindow);
	virtual void OnRender() override;
	
	TPointer<EditorScene> EditorScene;
};
