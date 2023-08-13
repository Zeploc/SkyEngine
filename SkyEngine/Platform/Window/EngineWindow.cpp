// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "EngineWindow.h"

#include "Core/Application.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Graphics/GraphicsAPI.h"
#include "Graphics/GraphicsInstance.h"
#include "Input/Input.h"
#include "Layers/Layer.h"
#include "Platform/PlatformInterface.h"
#include "System/TimeManager.h"

CEngineWindow::CEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen)
{
	WindowName = InWindowName;
	WindowSize = InWindowSize;
	bFullscreen = bInFullScreen;
}

CEngineWindow::~CEngineWindow()
{
	GraphicsInstance.reset();
	EventListeners.clear();	
		
	for (CLayer* Layer : LayerStack)
	{
		Layer->OnDetach();
	}
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

void CEngineWindow::PostRender()
{
	GraphicsInstance->PostRender(shared_from_this());
}

void CEngineWindow::Render()
{
	PreRender();
	for (CLayer* Layer : LayerStack)
	{
		Layer->OnRender();
	}
	PostRender();
}

void CEngineWindow::Update()
{
	if (CTimeManager::CanTickThisFrame())
	{
		// TODO: Once linking events system, would work backwards in layer based on highest first
		for (CLayer* Layer : LayerStack)
		{
			Layer->OnUpdate();
		}
	}
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
		CMouseButtonPressedEvent ButtonPressedEvent = CMouseButtonPressedEvent(button, mods);
		MouseEvent = &ButtonPressedEvent;
	}
	else
	{
		CMouseButtonReleasedEvent ButtonReleasedEvent = CMouseButtonReleasedEvent(button, mods);
		MouseEvent = &ButtonReleasedEvent;
	}

	CLayer* NewCapturedLayer = SendEvent(*MouseEvent);
	if (NewCapturedLayer)
	{
		if (EventType == CInput::Pressed)
		{
			CapturedLayer = NewCapturedLayer;
		}
		else if (NewCapturedLayer == CapturedLayer && MouseEvent->WasHandled())
		{
			CapturedLayer = nullptr;
		}
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

CLayer* CEngineWindow::SendEvent(CEvent& Event)
{
	for (IEventListener* EventListener : EventListeners)
	{
		EventListener->OnEvent(Event);
	}
	if (CapturedLayer)
	{
		CapturedLayer->OnEvent(Event);
		return CapturedLayer;
	}
	
	CLayer* HandledLayer = nullptr;
	for (auto it = LayerStack.end(); it != LayerStack.begin();)
	{
		(*--it)->OnEvent(Event);
		const bool bHandledEvent = Event.WasHandled();
		if (bHandledEvent)
		{
			HandledLayer = *it;
			break;
		}
	}
	return HandledLayer;
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

void CEngineWindow::PushLayer(CLayer* InLayer)
{
	LayerStack.PushLayer(InLayer);
	InLayer->OnAttach();
}

void CEngineWindow::PushOverlay(CLayer* InLayer)
{
	LayerStack.PushOverlay(InLayer);
	InLayer->OnAttach();
}

void CEngineWindow::OnFocusChanged(bool bInIsFocused)
{
	bIsFocused = bInIsFocused;
	// TODO: Param
	OnFocusChangedDelete.Broadcast();
}
