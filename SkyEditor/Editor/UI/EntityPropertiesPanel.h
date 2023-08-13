// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "UI/UIWidget.h"

class EditorScene;

class CEntityPropertiesPanel : public CUIWidget
{
public:
	CEntityPropertiesPanel();
	void DrawUI(const SCanvas& DrawCanvas) override;

	TPointer<EditorScene> EditorScene;
};
