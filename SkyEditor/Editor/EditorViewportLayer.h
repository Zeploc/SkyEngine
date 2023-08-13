// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Layers/ViewportLayer.h"

class CUIWidget;

class CEditorViewportLayer : public CViewportLayer
{
public:
	CEditorViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow);
	void OnUpdate() override;
	void OnRender() override;

	
	void AddViewportWidget(TPointer<CUIWidget> InWidget);

protected:
	std::vector<TPointer<CUIWidget>> Widgets;
};
