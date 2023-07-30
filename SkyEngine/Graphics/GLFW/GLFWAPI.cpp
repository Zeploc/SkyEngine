#include "GLFWAPI.h"

#include "GLFWInstance.h"
#include "GLFWWindow.h"

IGLFWAPI::IGLFWAPI()
{	
}

IGLFWAPI::~IGLFWAPI()
{	
	glfwTerminate();
}

std::string IGLFWAPI::GetGraphicsDisplayName()
{
	return "GLFW";
}

Pointer<IGraphicsWindow> IGLFWAPI::CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen)
{
	Pointer<GLFWWindow> NewWindow = std::make_shared<GLFWWindow>(InWindowName, InWindowSize, bFullScreen);
	if (!NewWindow->GetGlWindow())
	{
		NewWindow.reset();
	}

	NewWindow->GetGraphicsInstance()->WindowSetup(NewWindow);
	return NewWindow;
}
