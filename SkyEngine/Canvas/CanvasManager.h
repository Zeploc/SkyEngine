// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class CCanvas;
struct ImGuiContext;

class CCanvasManager
{
public:
	CCanvasManager();
	virtual ~CCanvasManager();
	void SetupCanvasManager();

	/* Pushed onto the end of the layers, but before the overlays*/
	void PushLayer(CCanvas* InLayer);
	/* Always pushed on top*/
	void PushOverlay(CCanvas* InOverlay);
	void PopLayer(CCanvas* InLayer);
	void PopOverlay(CCanvas* InOverlay);

	std::vector<CCanvas*>::iterator begin() { return Layers.begin(); }
	std::vector<CCanvas*>::iterator end() { return Layers.end(); }

	ImGuiContext* GetGuiContext() const { return GuiContext; }
private:
	std::vector<CCanvas*> Layers;
	unsigned int LayersInsertIndex = 0;
	
	ImGuiContext* GuiContext;
};
