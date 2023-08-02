#include "WindowsPlatform.h"

#include "Core/Application.h"
#include "Graphics/GraphicsInstance.h"
#include "Platform/Window/GLFW/GLFWWindow.h"

WindowsPlatform::~WindowsPlatform()
{
	// TODO: Not sure if this is the right place? Look into what this does, can't be done per window if its main gl
	glfwTerminate();
}

std::string WindowsPlatform::GetPlatformDisplayName()
{
	return "Windows";
}

TPointer<IGraphicsWindow> WindowsPlatform::CreateNewWindow(const std::string& InWindowName, SVector2 InWindowSize, bool bFullScreen)
{
	TPointer<GLFWWindow> NewWindow = std::make_shared<GLFWWindow>(InWindowName, InWindowSize, bFullScreen);
	if (!NewWindow->GetGlWindow())
	{
		NewWindow.reset();
	}
	return NewWindow;
}
