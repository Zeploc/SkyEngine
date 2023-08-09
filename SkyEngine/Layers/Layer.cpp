// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Layer.h"

CLayer::CLayer(TWeakPointer<CEngineWindow> InOwningWindow, const std::string& InDebugName)
: OwningWindow(InOwningWindow), DebugName(InDebugName)
{
}

CLayer::~CLayer()
{
}
