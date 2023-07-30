// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <string>

#include "Core/Core.h"

#include "Math/Vector2.h"

class IGraphicsInstance;
class UIElement;
class Entity;
class IGraphicsWindow;

enum class EGraphicsAPI
{
	GLFW,
	VULKAN
};

class ENGINE_API IGraphicsAPI
{
public:
	virtual ~IGraphicsAPI() = default;
	
	virtual std::string GetGraphicsDisplayName() = 0;
	
	virtual Pointer<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen) = 0;

	static Pointer<IGraphicsAPI> CreateGraphicsAPI(EGraphicsAPI APIType);
};
