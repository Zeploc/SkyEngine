// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "EngineWindow.h"

#include "Application.h"
#include "Graphics/GraphicsAPI.h"
#include "Input/Input.h"
#include "Platform/PlatformInterface.h"
#include "Platform/Window/GraphicsWindow.h"

EngineWindow::~EngineWindow()
{
	GraphicsWindow.reset();
}

TPointer<EngineWindow> EngineWindow::CreateEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen)
{
	TPointer<EngineWindow> NewWindow = std::make_shared<EngineWindow>(InWindowName, InWindowSize, bInFullScreen);
	if (!NewWindow->GetGraphicsWindow())
	{
		NewWindow.reset();
	}
	// The input function registration
	// Input::GetInstance()->Init(NewWindow);
	return NewWindow;
}

bool EngineWindow::ShouldWindowClose() const
{
	return GraphicsWindow->ShouldWindowClose();
}

SVector2i EngineWindow::GetSize()
{
	WindowSize = GraphicsWindow->GetWindowSize();
	return WindowSize;
}

SVector2i EngineWindow::GetPosition()
{
	WindowPosition = GraphicsWindow->GetWindowPosition();
	return WindowPosition;
}

void EngineWindow::SetWindowPosition(SVector2i InPosition)
{
	WindowPosition = InPosition;
	GraphicsWindow->SetWindowPosition(WindowPosition);
}

void EngineWindow::OnFocusedChanged()
{
	OnFocusChanged.Broadcast();
}

EngineWindow::EngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen)
{
	WindowName = InWindowName;
	WindowSize = InWindowSize;
	bFullscreen = bInFullScreen;
	
	GraphicsWindow = GetApplication()->PlatformInterface->CreateNewWindow(InWindowName, WindowSize, bInFullScreen);
	GraphicsWindow->CreateGraphicsInstance();
	GraphicsWindow->OnFocusChanged.Bind(this, &EngineWindow::OnFocusedChanged);		
}
