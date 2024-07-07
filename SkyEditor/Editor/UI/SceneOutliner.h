// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Canvas/UICanvas.h"
#include "Core/Object.h"

class Scene;

class CSceneOutliner : public CUICanvas
{
public:
	CSceneOutliner(TWeakPointer<CEngineWindow> InOwningWindow);
	virtual void OnRender() override;
	
	TAssetObjectPointer<Scene> EditorScene;
};
