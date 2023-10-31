// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "EngineWindow.h"

#include "Core/Application.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Input/Input.h"
#include "Canvas/Canvas.h"
#include "Render/Renderer.h"
#include "Platform/PlatformInterface.h"
#include "System/TimeManager.h"

CEngineWindow::CEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen)
{
	WindowName = InWindowName;
	WindowSize = InWindowSize;
	bFullscreen = bInFullScreen;
}

bool CEngineWindow::SetupWindow()
{
	// The input function registration
	Input.Init(shared_from_this());
	
	CanvasManager.SetupCanvasManager(shared_from_this());

	return true;
}

CEngineWindow::~CEngineWindow()
{
	EventListeners.clear();	
		
	for (CCanvas* Layer : CanvasManager)
	{
		Layer->OnDetach();
	}
}

TPointer<CEngineWindow> CEngineWindow::CreateEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen)
{	
	TPointer<CEngineWindow> NewWindow = GetApplication()->PlatformInterface->CreateNewWindow(InWindowName, InWindowSize, bInFullScreen);
	if (!NewWindow->SetupWindow())
	{	
		NewWindow.reset();
	}
	
	return NewWindow;
}


void CEngineWindow::PreRender()
{
}

void CEngineWindow::PostRender()
{
}

void CEngineWindow::Render()
{
	PreRender();
	GetRenderer()->RenderScenes();
	CanvasManager.Render();
	PostRender();
}

void CEngineWindow::Update()
{
	if (CTimeManager::CanTickThisFrame())
	{
		CanvasManager.Update();
	}
	Input.Update();
}

void CEngineWindow::SetCursorVisible(bool bSetVisible)
{
	bCursorVisible = bSetVisible;
}

void CEngineWindow::MouseButtonPress(int button, CWindowInput::KeyEventType EventType, int mods)
{
	Input.MouseButton(button, EventType, mods);
	CMouseButtonEvent* MouseEvent;
	if (EventType == CWindowInput::Pressed)
	{
		CMouseButtonPressedEvent ButtonPressedEvent = CMouseButtonPressedEvent(button, mods);
		MouseEvent = &ButtonPressedEvent;
	}
	else
	{
		CMouseButtonReleasedEvent ButtonReleasedEvent = CMouseButtonReleasedEvent(button, mods);
		MouseEvent = &ButtonReleasedEvent;
	}

	SendEvent(*MouseEvent);
}

// TODO: Mod type
void CEngineWindow::KeyPress(int key, int scancode, CWindowInput::KeyEventType EventType, int mods)
{
	Input.ProcessKeys(key, scancode, EventType, mods);

	CKeyEvent* KeyEvent;
	if (EventType == CWindowInput::Pressed)
	{
		CKeyPressedEvent KeyPressedEvent = CKeyPressedEvent(key, mods, 0);
		KeyEvent = &KeyPressedEvent;
	}
	else if (EventType == CWindowInput::Repeat)
	{
		CKeyPressedEvent KeyPressedEvent = CKeyPressedEvent(key, mods, 1);
		KeyEvent = &KeyPressedEvent;
	}
	else
	{
		CKeyReleasedEvent KeyPressedEvent = CKeyReleasedEvent(key, mods);
		KeyEvent = &KeyPressedEvent;
	}
	
	SendEvent(*KeyEvent);
}

void CEngineWindow::KeyTyped(int KeyCode)
{
	CKeyTypedEvent KeyTypedEvent(KeyCode, 0);
	SendEvent(KeyTypedEvent);
}

void CEngineWindow::CursorMoved(int X, int Y)
{
	Input.MouseInput(X, Y);
	CMouseMovedEvent MouseEvent(SVector2i(X, Y));
	SendEvent(MouseEvent);
}

void CEngineWindow::ScrollWheel(float X, float Y)
{
	CMouseScrolledEvent ScrollEvent(X, Y);
	SendEvent(ScrollEvent);
}

void CEngineWindow::OnWindowResized(int NewWidth, int NewHeight)
{
	WindowSize = {NewWidth, NewHeight};
	// TODO: Delegate?
	CWindowResizeEvent ResizeEvent(NewWidth, NewHeight);
	SendEvent(ResizeEvent);
}

void CEngineWindow::SendEvent(CEvent& Event)
{
	for (IEventListener* EventListener : EventListeners)
	{
		EventListener->OnEvent(Event);
	}
	
	CanvasManager.OnEvent(Event);
}

void CEngineWindow::SetWindowPosition(SVector2i InPosition)
{
	WindowPosition = InPosition;
}

void CEngineWindow::SetCursorPosition(SVector2i InCursorPosition)
{
	Input.MouseInput(InCursorPosition.x, InCursorPosition.y);
}

SVector2i CEngineWindow::GetScreenHalfSize()
{
	return SVector2i(GetSize().X / 2, GetSize().Y / 2);
}

void CEngineWindow::SubscribeEventListener(IEventListener* NewEventListener)
{
	EventListeners.push_back(NewEventListener);
}

void CEngineWindow::PushLayer(CCanvas* InLayer)
{
	CanvasManager.PushLayer(InLayer);
	InLayer->OnAttach();
}

void CEngineWindow::PushOverlay(CCanvas* InLayer)
{
	CanvasManager.PushOverlay(InLayer);
	InLayer->OnAttach();
}

void CEngineWindow::OnFocusChanged(bool bInIsFocused)
{
	bIsFocused = bInIsFocused;
	// TODO: Param
	OnFocusChangedDelete.Broadcast();
}
