// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "UIWidget.h"

#include "imgui.h"
#include "Canvas/Canvas.h"

void CUIWidget::SetOwningLayer(CCanvas* InCanvas)
{
	OwningCanvas = InCanvas;
}
