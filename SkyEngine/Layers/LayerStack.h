// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class CLayer;

class CLayerStack
{
public:
	CLayerStack();
	virtual ~CLayerStack();

	/* Pushed onto the end of the layers, but before the overlays*/
	void PushLayer(CLayer* InLayer);
	/* Always pushed on top*/
	void PushOverlay(CLayer* InOverlay);
	void PopLayer(CLayer* InLayer);
	void PopOverlay(CLayer* InOverlay);

	std::vector<CLayer*>::iterator begin() { return Layers.begin(); }
	std::vector<CLayer*>::iterator end() { return Layers.end(); }

private:
	std::vector<CLayer*> Layers;
	std::vector<CLayer*>::iterator LayersInsert;
	
};
