// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <memory>
#include <vector>

#include "Math/Vector.h"

class IGraphicsInstance;
class UIElement;
class Entity;
class IGraphicsWindow;

class ENGINE_API IGraphicsInterface
{
public:
	virtual ~IGraphicsInterface() = default;
	
	virtual std::string GetGraphicsDisplayName() = 0;
	
	virtual std::shared_ptr<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen) = 0;

	
};
