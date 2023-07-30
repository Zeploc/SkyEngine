#include "GLFWWindow.h"

#include <iostream>
#include <glew/glew.h>
#include <Render/Shader.h>

#include "Input/CXBOXController.h"
#include "Input/Input.h"

void glfw_onError(int error, const char * description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void FocusChanged(struct GLFWwindow* window, int focused);

GLFWWindow::~GLFWWindow()
{
	glfwDestroyWindow(GlWindow);
	GlWindow = nullptr;
}

GLFWWindow::GLFWWindow(std::string InWindowName, Vector2 InWindowSize, bool bFullScreen) : IGraphicsWindow(InWindowName, InWindowSize, bFullScreen)
{	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	
	// Enable debugging context
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwSetErrorCallback(glfw_onError);
	
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
	glfwSwapBuffers(GlWindow);
	glfwPollEvents();
}

void GLFWWindow::FocusWindow() const
{
	// TODO: Is this correct?
	
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

void glfw_onError(int error, const char* description)
{
	// print message in Windows popup dialog box
	MessageBox(NULL, LPCWSTR(description), LPCWSTR("GLFW error"), MB_OK);
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