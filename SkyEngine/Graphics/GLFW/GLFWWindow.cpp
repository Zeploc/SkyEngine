#include "GLFWWindow.h"

#include <iostream>
#include <glew/glew.h>
#include <Render/Shader.h>

#include "GLFWInstance.h"
#include "Input/Input.h"

GLFWWindow::~GLFWWindow()
{
	
}

GLFWWindow::GLFWWindow(std::string InWindowName, Vector2 InWindowSize, bool bFullScreen): IGraphicsWindow(InWindowName, InWindowSize, bFullScreen)
{
	// Create instance for basic setup before window
	GraphicsInstance = std::make_shared<GLFWInstance>();
	
	GLFWmonitor* FullscreenMonitor = nullptr;
	Vector2 WindowPosition;
	if (bFullScreen)
	{
		int* MonitorCount = new int;
		GLFWmonitor** GlfwGetMonitors = glfwGetMonitors(MonitorCount);
		FullscreenMonitor = GlfwGetMonitors[0];
	}
	else
	{
		int MonitorCount;
		GLFWmonitor** GlfwGetMonitors = glfwGetMonitors(&MonitorCount);
		int x, y, width, height;
		glfwGetMonitorWorkarea(GlfwGetMonitors[0], &x, &y, &width, &height);
		WindowPosition.X = width / 2.0f - InWindowSize.X / 2.0f;
		WindowPosition.Y = height / 2.0f - InWindowSize.Y / 2.0f;
	}
	GlWindow = glfwCreateWindow(InWindowSize.X, InWindowSize.Y, InWindowName.c_str(), FullscreenMonitor, nullptr);
	if (GlWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return;// -1;
	}

	if (!bFullScreen)
	{
		glfwSetWindowPos(GlWindow, WindowPosition.X, WindowPosition.Y);
	}
	glViewport(0, 0, InWindowSize.X, InWindowSize.Y);	
}

void GLFWWindow::SetWindowFullScreen(bool bFullscreen)
{
	// TODO:
}

bool GLFWWindow::ShouldWindowClose() const
{	
	return glfwWindowShouldClose(GlWindow);
}

void GLFWWindow::FocusWindow() const
{
	// TODO: Is this correct?
	glfwMakeContextCurrent(GlWindow);
}

void GLFWWindow::SetCursorPosition(Vector2 InCursorPosition)
{	
	glfwSetCursorPos(GlWindow, InCursorPosition.X, InCursorPosition.Y);
}

bool GLFWWindow::CloseWindow()
{
	glfwSetWindowShouldClose(GlWindow, true);
	return true;
}

Vector2 GLFWWindow::GetWindowPosition()
{
	// TODO: Swap to Vector2i
	int x, y;
	glfwGetWindowPos(GlWindow, &x, &y);
	return {static_cast<float>(x), static_cast<float>(y)};
}

void GLFWWindow::SetWindowPosition(Vector2 InWindowPosition)
{
	glfwSetWindowPos(GlWindow, InWindowPosition.X, InWindowPosition.Y);
}

Vector2 GLFWWindow::GetWindowSize()
{
	// TODO: Swap to Vector2i
	int WindowWidth, WindowHeight;
	glfwGetWindowSize(GlWindow, &WindowWidth, &WindowHeight);
	return {static_cast<float>(WindowWidth), static_cast<float>(WindowHeight)};
}