// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

#include "Core/Delegates.h"
#include "Input/Input.h"
#include "Math/Vector2.h"

class IGraphicsWindow;
class IEventListener;
class IGraphicsInstance;
class UIElement;
class Entity;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API CEngineWindow : public std::enable_shared_from_this<CEngineWindow>
{
public:
	CEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen);
	virtual ~CEngineWindow();
	static TPointer<CEngineWindow> CreateEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen = false);
	virtual void CreateGraphicsInstance();
	std::string GetWindowName() { return WindowName; }

	virtual void SetWindowFullScreen(bool bFullScreen) = 0;
	virtual bool ShouldWindowClose() const { return false; }
	virtual bool CloseWindow() = 0;
	virtual void FocusWindow() const = 0;
	
	virtual void SetCursorVisible(bool bSetVisible);	

	virtual SVector2i GetSize() const { return WindowSize; }
	virtual SVector2i GetPosition() const { return WindowPosition; }
	virtual void SetWindowPosition(SVector2i InPosition);	
	virtual void SetCursorPosition(SVector2i InCursorPosition);

	SVector2i GetScreenHalfSize();
	
	virtual void PreRender();
	void Render(std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements);
	virtual void PostRender();
	virtual void Update();

	void SubscribeEventListener(IEventListener* NewEventListener);

	// TODO: Pass in window in delegate
	FMulticastDelegate OnFocusChangedDelete;	

	TPointer<IGraphicsInstance> GetGraphicsInstance() { return GraphicsInstance; }
	CInput& GetInput() { return Input; }

protected:
	virtual void OnWindowResized(int NewWidth, int NewHeight) = 0;
	virtual void OnFrameBufferResized(int NewWidth, int NewHeight) = 0;
	virtual void OnFocusChanged(bool bIsFocused);
	
	virtual void MouseButtonPress(int button, CInput::KeyEventType EventType, int mods);
	virtual void KeyPress(int key, int scancode, CInput::KeyEventType EventType, int mods);
	virtual void CursorMoved(int X, int Y);
	virtual void ScrollWheel(float X, float Y);
	
	TPointer<IGraphicsInstance> GraphicsInstance;
	std::string WindowName;
	SVector2i WindowSize;
	SVector2i WindowPosition;
	bool bFullscreen = false;
	bool bCursorVisible = true;
	CInput Input;
	
	std::vector<IEventListener*> EventListeners;
};
