// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "EngineWindow.h"

#include "Core/Application.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/GraphicsInstance.h"
#include "Input/Input.h"
#include "Platform/PlatformInterface.h"

CEngineWindow::~CEngineWindow()
{
	GraphicsInstance.reset();
	EventListeners.clear();
}

TPointer<CEngineWindow> CEngineWindow::CreateEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen)
{	
	TPointer<CEngineWindow> NewWindow = GetApplication()->PlatformInterface->CreateNewWindow(InWindowName, InWindowSize, bInFullScreen);
	NewWindow->CreateGraphicsInstance();	
	if (!NewWindow->GetGraphicsInstance())
	{
		NewWindow.reset();
	}
	// The input function registration
	NewWindow->Input.Init(NewWindow);
	return NewWindow;
}

void CEngineWindow::CreateGraphicsInstance()
{
	GraphicsInstance = GetGraphicsAPI()->CreateNewInstance();
}

void CEngineWindow::PreRender()
{
	GraphicsInstance->PreRender(shared_from_this());
}

void CEngineWindow::Render(std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements)
{
	GraphicsInstance->Render(shared_from_this(), Entities, UIElements);
}

void CEngineWindow::PostRender()
{
	GraphicsInstance->PostRender(shared_from_this());
}

void CEngineWindow::Update()
{
	Input.Update();
}

void CEngineWindow::SetCursorVisible(bool bSetVisible)
{
	bCursorVisible = bSetVisible;
}

void CEngineWindow::MouseButtonPress(int button, CInput::KeyEventType EventType, int mods)
{
	Input.MouseButton(button, EventType, mods);
	CMouseButtonEvent* MouseEvent;
	if (EventType == CInput::Pressed)
	{
		CMouseButtonPressedEvent KeyPressedEvent = CMouseButtonPressedEvent(button, mods);
		MouseEvent = &KeyPressedEvent;
	}
	else
	{
		CMouseButtonReleasedEvent KeyPressedEvent = CMouseButtonReleasedEvent(button, mods);
		MouseEvent = &KeyPressedEvent;
	}
	
	for (IEventListener* EventListener : EventListeners)
	{
		EventListener->OnEvent(*MouseEvent);
	}
}

// TODO: Mod type
void CEngineWindow::KeyPress(int key, int scancode, CInput::KeyEventType EventType, int mods)
{
	Input.ProcessKeys(key, scancode, EventType, mods);

	CKeyEvent* KeyEvent;
	if (EventType == CInput::Pressed)
	{
		CKeyPressedEvent KeyPressedEvent = CKeyPressedEvent(key, mods, 0);
		KeyEvent = &KeyPressedEvent;
	}
	else if (EventType == CInput::Repeat)
	{
		CKeyPressedEvent KeyPressedEvent = CKeyPressedEvent(key, mods, 1);
		KeyEvent = &KeyPressedEvent;
	}
	else
	{
		CKeyReleasedEvent KeyPressedEvent = CKeyReleasedEvent(key, mods);
		KeyEvent = &KeyPressedEvent;
	}
	
	for (IEventListener* EventListener : EventListeners)
	{
		EventListener->OnEvent(*KeyEvent);
	}
}

void CEngineWindow::CursorMoved(int X, int Y)
{
	Input.MouseInput(X, Y);
	CMouseMovedEvent MouseEvent(SVector2i(X, Y));
	for (IEventListener* EventListener : EventListeners)
	{
		EventListener->OnEvent(MouseEvent);
	}
}

void CEngineWindow::ScrollWheel(float X, float Y)
{
	CMouseScrolledEvent ScrollEvent(X, Y);
	for (IEventListener* EventListener : EventListeners)
	{
		EventListener->OnEvent(ScrollEvent);
	}
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

void CEngineWindow::OnFocusChanged(bool bInIsFocused)
{
	bIsFocused = bInIsFocused;
	// TODO: Param
	OnFocusChangedDelete.Broadcast();
}

CEngineWindow::CEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen)
{
	WindowName = InWindowName;
	WindowSize = InWindowSize;
	bFullscreen = bInFullScreen;
}
