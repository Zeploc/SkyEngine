#include "WindowsPlatform.h"

#include "Core/Application.h"
#include "Graphics/GraphicsInstance.h"
#include "Platform/Window/GLFW/GLFWWindow.h"

std::string WindowsPlatform::GetPlatformDisplayName()
{
	return "Windows";
}

Pointer<IGraphicsWindow> WindowsPlatform::CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen)
{
	Pointer<GLFWWindow> NewWindow = std::make_shared<GLFWWindow>(InWindowName, InWindowSize, bFullScreen);
	if (!NewWindow->GetGlWindow())
	{
		NewWindow.reset();
	}
	return NewWindow;
}
