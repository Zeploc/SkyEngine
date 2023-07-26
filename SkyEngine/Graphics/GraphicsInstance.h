// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <memory>
#include <vector>

#include "Math/Vector.h"

class UIElement;
class Entity;
class IGraphicsWindow;

class IGraphicsInstance
{
public:
	virtual ~IGraphicsInstance() = default;

	virtual void WindowSetup(std::shared_ptr<IGraphicsWindow> InGraphicsWindow) = 0;

	virtual void SwapBuffers(std::shared_ptr<IGraphicsWindow> GraphicsWindow) = 0;

	virtual void Render(std::shared_ptr<IGraphicsWindow> GraphicsWindow, std::vector<std::shared_ptr<Entity>> Entities, std::vector<std::shared_ptr<UIElement>> UIElements) = 0;
	
	virtual void PreRender(std::shared_ptr<IGraphicsWindow> GraphicsWindow) {}
	virtual void PostRender(std::shared_ptr<IGraphicsWindow> GraphicsWindow) {}
	
	// TODO: Create colour type
	Vector3 ClearColour = Vector3(0.3f, 0.8f, 0.9f);
};
