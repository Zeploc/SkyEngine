// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "UI/UIWidget.h"

class EditorScene;

class CSceneOutliner : public CUIWidget
{
public:
	CSceneOutliner();
	void DrawUI(const SCanvas& DrawCanvas) override;
	
	TPointer<EditorScene> EditorScene;
};
