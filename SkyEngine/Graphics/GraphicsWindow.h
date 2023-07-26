// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <memory>
#include <vector>

#include "Core/Delegates.h"
#include "Math/Vector.h"

class UIElement;
class Entity;
class IGraphicsInstance;

class ENGINE_API IGraphicsWindow : public std::enable_shared_from_this<IGraphicsWindow>
{
public:
	virtual ~IGraphicsWindow() = default;

	// TODO: Convert to Vector2i
	IGraphicsWindow(std::string InWindowName, Vector2 InWindowSize, bool bFullScreen) {}

	virtual void SetWindowFullScreen(bool bFullScreen) = 0;
	virtual bool ShouldWindowClose() const { return false; }
	virtual bool CloseWindow() = 0;
	virtual void FocusWindow() const = 0;
	virtual void SetCursorPosition(Vector2 InCursorPosition) = 0;
	virtual void SetWindowPosition(Vector2 InWindowPosition) = 0;
	virtual Vector2 GetWindowPosition() = 0;
	virtual Vector2 GetWindowSize() = 0;
	
	virtual void PreRender();
	void Render(std::vector<std::shared_ptr<Entity>> Entities, std::vector<std::shared_ptr<UIElement>> UIElements);
	virtual void PostRender();
	
	std::shared_ptr<IGraphicsInstance> GetGraphicsInstance() { return GraphicsInstance; }

	FDelegate OnFocusChanged;

protected:
	std::shared_ptr<IGraphicsInstance> GraphicsInstance;
};

