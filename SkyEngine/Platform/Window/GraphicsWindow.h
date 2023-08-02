// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <memory>
#include <vector>

#include "Core/Delegates.h"
#include "Math/Vector2.h"

class UIElement;
class Entity;
class IGraphicsInstance;

class ENGINE_API IGraphicsWindow : public std::enable_shared_from_this<IGraphicsWindow>
{
public:
	virtual ~IGraphicsWindow() = default;

	// TODO: Convert to Vector2i
	IGraphicsWindow(std::string InWindowName, SVector2 InWindowSize, bool bFullScreen);
	virtual void CreateGraphicsInstance();

	virtual void SetWindowFullScreen(bool bFullScreen) = 0;
	virtual bool ShouldWindowClose() const { return false; }
	virtual bool CloseWindow() = 0;
	virtual void FocusWindow() const = 0;
	virtual void SetCursorPosition(SVector2 InCursorPosition);
	virtual void SetWindowPosition(SVector2 InWindowPosition) = 0;
	virtual SVector2 GetWindowPosition() = 0;
	virtual SVector2 GetWindowSize() = 0;
	
	virtual void PreRender();
	void Render(std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements);
	virtual void PostRender();

	TPointer<IGraphicsInstance> GetGraphicsInstance() { return GraphicsInstance; }

	FDelegate OnFocusChanged;

protected:
	TPointer<IGraphicsInstance> GraphicsInstance;
};

