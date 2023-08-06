// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "LayerStack.h"

#include "Layer.h"

CLayerStack::CLayerStack()
{
	LayersInsert = Layers.begin();
}

CLayerStack::~CLayerStack()
{
	for (const CLayer* Layer : Layers)
	{
		delete Layer;
	}
}

void CLayerStack::PushLayer(CLayer* InLayer)
{
	LayersInsert = Layers.emplace(LayersInsert, InLayer);
}

void CLayerStack::PushOverlay(CLayer* InOverlay)
{
	Layers.emplace_back(InOverlay);
}

void CLayerStack::PopLayer(CLayer* InLayer)
{
	// Find layer
	const auto it = std::find(Layers.begin(), Layers.end(), InLayer);
	// If found
	if (it != Layers.end())
	{
		Layers.erase(it);
		LayersInsert--;
	}
}

void CLayerStack::PopOverlay(CLayer* InOverlay)
{
	// Find layer
	const auto it = std::find(Layers.begin(), Layers.end(), InOverlay);
	// If found
	if (it != Layers.end())
	{
		Layers.erase(it);
	}
}
