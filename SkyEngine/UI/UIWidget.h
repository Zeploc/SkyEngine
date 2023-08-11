// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

class CUILayer;

class ENGINE_API CUIWidget
{
public:
	virtual void DrawUI() = 0;
	virtual void Update() {}

	void SetUILayer(CUILayer* InLayer);

protected:
	CUILayer* OwningLayer = nullptr;
	
};
