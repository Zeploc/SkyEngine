// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"
#include "Math/Internal/Vector2.decl.h"

class CLayer;

struct ENGINE_API SCanvas
{
	SVector2 Position;
	SVector2 Size;	
};

class ENGINE_API CUIWidget
{
public:
	virtual void DrawUI(const SCanvas& DrawCanvas) = 0;
	virtual void Update() {}

	void SetOwningLayer(CLayer* InLayer);

protected:
	CLayer* OwningLayer = nullptr;
	
};
