// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Delegates.h"
#include "Math/Vector2.h"

class UIElement;
class Entity;
class IGraphicsInstance;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API IGraphicsWindow : public std::enable_shared_from_this<IGraphicsWindow>
{
public:
	virtual ~IGraphicsWindow() = default;

	// TODO: Convert to Vector2i
	IGraphicsWindow(std::string InWindowName, SVector2i InWindowSize, bool bFullScreen);
	virtual void CreateGraphicsInstance();

	virtual void SetWindowFullScreen(bool bFullScreen) = 0;
	virtual bool ShouldWindowClose() const { return false; }
	virtual bool CloseWindow() = 0;
	virtual void FocusWindow() const = 0;
	virtual void SetCursorPosition(SVector2i InCursorPosition);
	virtual void SetWindowPosition(SVector2i InWindowPosition) = 0;
	virtual SVector2i GetWindowPosition() = 0;
	virtual SVector2i GetWindowSize() = 0;
	
	virtual void PreRender();
	void Render(std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements);
	virtual void PostRender();

	TPointer<IGraphicsInstance> GetGraphicsInstance() { return GraphicsInstance; }

	FDelegate OnFocusChanged;

protected:
	TPointer<IGraphicsInstance> GraphicsInstance;
};

