// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <string>

#include "Core/Core.h"

#include "Math/Vector2.h"

class IGraphicsInstance;
class UIElement;
class Entity;
class IGraphicsWindow;

class ENGINE_API IGraphicsInterface
{
public:
	virtual ~IGraphicsInterface() = default;
	
	virtual std::string GetGraphicsDisplayName() = 0;
	
	virtual Pointer<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen) = 0;
	
};
