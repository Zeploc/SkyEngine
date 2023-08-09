// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "EngineWindow.h"

#include "Core/Application.h"
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
	Input::GetInstance()->Init(NewWindow);
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
	Input::GetInstance()->Update();
}

void CEngineWindow::SetCursorVisible(bool bSetVisible)
{
	bCursorVisible = bSetVisible;
}

void CEngineWindow::MouseButtonPress(int button, Input::KeyEventType EventType, int mods)
{
	Input::GetInstance()->MouseButton(button, EventType, mods);
}

// TODO: Mod type
void CEngineWindow::KeyPress(int key, int scancode, Input::KeyEventType EventType, int mods)
{
	Input::GetInstance()->ProcessKeys(key, scancode, EventType, mods);
}

void CEngineWindow::CursorMoved(int X, int Y)
{
	Input::GetInstance()->MouseInput(X, Y);
}

void CEngineWindow::ScrollWheel(int X, int Y)
{
	// TODOO:
}

void CEngineWindow::SetWindowPosition(SVector2i InPosition)
{
	WindowPosition = InPosition;
}

void CEngineWindow::SubscribeEventListener(IEventListener* NewEventListener)
{
	EventListeners.push_back(NewEventListener);
}

void CEngineWindow::OnFocusChanged(bool bIsFocused)
{
	// TODO: Param
	OnFocusChangedDelete.Broadcast();
}

CEngineWindow::CEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen)
{
	WindowName = InWindowName;
	WindowSize = InWindowSize;
	bFullscreen = bInFullScreen;
}
