#include "GLFWInterface.h"

#include "GLFWInstance.h"
#include "GLFWWindow.h"

GLFWInterface::GLFWInterface()
{	
}

GLFWInterface::~GLFWInterface()
{	
	glfwTerminate();
}

std::string GLFWInterface::GetGraphicsDisplayName()
{
	return "GLFW";
}

Pointer<IGraphicsWindow> GLFWInterface::CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen)
{
	Pointer<GLFWWindow> NewWindow = std::make_shared<GLFWWindow>(InWindowName, InWindowSize, bFullScreen);
	if (!NewWindow->GetGlWindow())
	{
		NewWindow.reset();
	}

	NewWindow->GetGraphicsInstance()->WindowSetup(NewWindow);
	return NewWindow;
}
