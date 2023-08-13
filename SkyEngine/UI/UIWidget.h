// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

class CLayer;

class ENGINE_API CUIWidget
{
public:
	virtual void DrawUI() = 0;
	virtual void Update() {}

	void SetOwningLayer(CLayer* InLayer);

protected:
	CLayer* OwningLayer = nullptr;
	
};
