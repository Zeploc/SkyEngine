// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GLFWWindow.h"

#include <iostream>
#include <glew/glew.h>
#include <Render/Shaders/ShaderManager.h>

#include "Input/CXBOXController.h"
#include "Input/Input.h"
#include "System/LogManager.h"

void glfw_onError(int error, const char * description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void FocusChanged(struct GLFWwindow* window, int focused);

GLFWWindow::~GLFWWindow()
{
	glfwDestroyWindow(GlWindow);
	GlWindow = nullptr;
}

GLFWWindow::GLFWWindow(std::string InWindowName, SVector2i InWindowSize, bool bFullScreen) : IGraphicsWindow(InWindowName, InWindowSize, bFullScreen)
{
	// TODO: Only call below once for whole program
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	
	// Enable debugging context
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwSetErrorCallback(glfw_onError);
	
	GLFWmonitor* FullscreenMonitor = nullptr;
	SVector2i WindowPosition;
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
		WindowPosition.X = width / 2 - InWindowSize.X / 2;
		WindowPosition.Y = height / 2 - InWindowSize.Y / 2;
	}
	GlWindow = glfwCreateWindow(InWindowSize.X, InWindowSize.Y, InWindowName.c_str(), FullscreenMonitor, nullptr);			
	if (!ENSURE_CORE(GlWindow != NULL, "Failed to create GLFW window"))
	{
		return;// -1;
	}
	glfwMakeContextCurrent(GlWindow);

	if (!bFullScreen)
	{
		glfwSetWindowPos(GlWindow, WindowPosition.X, WindowPosition.Y);
	}
	
	// TODO: link properly
	glfwSetFramebufferSizeCallback(GlWindow, framebuffer_size_callback);	
	glfwSetWindowFocusCallback(GlWindow, FocusChanged);
}

void GLFWWindow::CreateGraphicsInstance()
{
	glfwMakeContextCurrent(GlWindow);
	IGraphicsWindow::CreateGraphicsInstance();
}

void GLFWWindow::SetWindowFullScreen(bool bFullscreen)
{
	// TODO:
}

bool GLFWWindow::ShouldWindowClose() const
{	
	return glfwWindowShouldClose(GlWindow);
}

void GLFWWindow::PreRender()
{
	glfwMakeContextCurrent(GlWindow);	
	IGraphicsWindow::PreRender();
}

void GLFWWindow::PostRender()
{
	IGraphicsWindow::PostRender();
	glfwPollEvents();
	glfwSwapBuffers(GlWindow);
}

// TODO: VSync with glfwSwapInterval(1);

void GLFWWindow::FocusWindow() const
{
	// TODO: Is this correct?
	
}

void GLFWWindow::SetCursorPosition(SVector2i InCursorPosition)
{
	IGraphicsWindow::SetCursorPosition(InCursorPosition);
	
	glfwSetCursorPos(GlWindow, InCursorPosition.X, InCursorPosition.Y);
}

bool GLFWWindow::CloseWindow()
{
	glfwSetWindowShouldClose(GlWindow, true);
	return true;
}

SVector2i GLFWWindow::GetWindowPosition()
{
	// TODO: Swap to Vector2i
	int x, y;
	glfwGetWindowPos(GlWindow, &x, &y);
	return {x, y};
}

void GLFWWindow::SetWindowPosition(SVector2i InWindowPosition)
{
	glfwSetWindowPos(GlWindow, InWindowPosition.X, InWindowPosition.Y);
}

SVector2i GLFWWindow::GetWindowSize()
{
	// TODO: Swap to Vector2i
	int WindowWidth, WindowHeight;
	glfwGetWindowSize(GlWindow, &WindowWidth, &WindowHeight);
	return {WindowWidth, WindowHeight};
}

void glfw_onError(int error, const char* description)
{
	// print message in Windows popup dialog box
	LogManager::GetInstance()->DisplayLogError(description);
	MessageBoxA(NULL, description, "GLFW error", MB_OK);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void FocusChanged(GLFWwindow* window, int focused)
{
	if (focused == GLFW_TRUE)
	{
		// TODO: Focus changed link
		//OnFocusChanged.Broadcast();
	}
}