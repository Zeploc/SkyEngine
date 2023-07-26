#include "EngineWindow.h"

#include "Application.h"
#include "Graphics/GraphicsInterface.h"
#include "Graphics/GraphicsWindow.h"
#include "Input/Input.h"

EngineWindow::~EngineWindow()
{
	GraphicsWindow.reset();
}

std::shared_ptr<EngineWindow> EngineWindow::CreateEngineWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bInFullScreen)
{
	std::shared_ptr<EngineWindow> NewWindow = std::make_shared<EngineWindow>(InWindowName, InWindowSize, bInFullScreen);
	if (!NewWindow->GetGraphicsWindow())
	{
		NewWindow.reset();
	}
	// The input function registration
	Input::GetInstance()->Init(NewWindow);
	return NewWindow;
}

bool EngineWindow::ShouldWindowClose() const
{
	return GraphicsWindow->ShouldWindowClose();
}

Vector2 EngineWindow::GetSize()
{
	WindowSize = GraphicsWindow->GetWindowSize();
	return WindowSize;
}

Vector2 EngineWindow::GetPosition()
{
	WindowPosition = GraphicsWindow->GetWindowPosition();
	return WindowPosition;
}

void EngineWindow::SetWindowPosition(Vector2 InPosition)
{
	WindowPosition = InPosition;
	GraphicsWindow->SetWindowPosition(WindowPosition);
}

void EngineWindow::OnFocusedChanged()
{
	OnFocusChanged.Broadcast();
}

EngineWindow::EngineWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bInFullScreen)
{
	WindowName = InWindowName;
	WindowSize = InWindowSize;
	bFullscreen = bInFullScreen;
	
	GraphicsWindow = GetApplication()->GraphicsInterface->CreateNewWindow(InWindowName, WindowSize, bInFullScreen);
	GraphicsWindow->OnFocusChanged.Bind(this, &EngineWindow::OnFocusedChanged);		
}
