// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

#include "Core/Delegates.h"
#include "Events/Event.h"
#include "Input/Input.h"
#include "Canvas/CanvasManager.h"
#include "Math/Vector2.h"

class IGraphicsWindow;
class IEventListener;
class UIElement;
class Entity;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API CEngineWindow : public std::enable_shared_from_this<CEngineWindow>
{
public:
	CEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen);
	bool SetupWindow();
	virtual ~CEngineWindow();
	static THardPointer<CEngineWindow> CreateEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen = false);
	std::string GetWindowName() { return WindowName; }

	virtual void SetWindowFullScreen(bool bFullScreen) = 0;
	virtual bool ShouldWindowClose() const { return false; }
	virtual void SetWindowTitle(const std::string& NewTitle) = 0;
	virtual bool CloseWindow() = 0;
	virtual void FocusWindow() const = 0;
	
	virtual void SetCursorVisible(bool bSetVisible);	

	virtual SVector2i GetSize() const { return WindowSize; }
	virtual SVector2i GetPosition() const { return WindowPosition; }
	virtual void SetWindowPosition(SVector2i InPosition);	
	virtual void SetCursorPosition(SVector2i InCursorPosition);
	bool IsFocused() const { return bIsFocused; }

	SVector2i GetScreenHalfSize();

	void Render();
	
	// void Render(std::vector<THardPointer<Entity>> Entities, std::vector<THardPointer<UIElement>> UIElements);
	virtual void Update();

	void SubscribeEventListener(IEventListener* NewEventListener);
	void PushLayer(CCanvas* InLayer);
	void PushOverlay(CCanvas* InLayer);
	template<class T>
	T* GetCanvas();

	// TODO: Pass in window in delegate
	FMulticastDelegate OnFocusChangedDelete;	

	CWindowInput& GetInput() { return Input; }
	CCanvasManager& GetCanvasManager() { return CanvasManager; }

protected:
	virtual void PreRender();
	virtual void PostRender();
	virtual void OnWindowResized(int NewWidth, int NewHeight);
	virtual void OnFrameBufferResized(int NewWidth, int NewHeight) = 0;
	virtual void OnFocusChanged(bool bInIsFocused);
	
	virtual void MouseButtonPress(int button, CWindowInput::KeyEventType EventType, int mods);
	virtual void KeyPress(int key, int scancode, CWindowInput::KeyEventType EventType, int mods);
	virtual void KeyTyped(int KeyCode);
	virtual void CursorMoved(int X, int Y);
	virtual void ScrollWheel(float X, float Y);

	/* Returns the handled layer, if there was one */
	virtual void SendEvent(CEvent& Event);
	
	std::string WindowName;
	SVector2i WindowSize;
	SVector2i WindowPosition;
	bool bFullscreen = false;
	bool bCursorVisible = true;
	bool bIsFocused = true;
	CWindowInput Input;
	
	CCanvasManager CanvasManager;
	std::vector<IEventListener*> EventListeners;
};

template <class T>
T* CEngineWindow::GetCanvas()
{
	for (CCanvas* Canvas : CanvasManager)
	{
		if (T* FoundCanvas = dynamic_cast<T*>(Canvas))
		{
			return FoundCanvas;
		}
	}
	return nullptr;
}
