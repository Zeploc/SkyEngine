// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UIWidget.h"

#include "imgui.h"
#include "Layers/UILayer.h"

void CUIWidget::SetOwningLayer(CLayer* InLayer)
{
	OwningLayer = InLayer;
}
