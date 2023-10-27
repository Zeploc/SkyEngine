// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GLFWWindow.h"

#include <iostream>
#include <glew/glew.h>
#include <Render/Shaders/ShaderManager.h>

#include "Graphics/Framebuffer.h"
#include "Graphics/GraphicsInstance.h"
#include "Input/CXBOXController.h"
#include "Input/Input.h"
#include "System/LogManager.h"

void glfw_onError(int error, const char * description);

CGLFWWindow::~CGLFWWindow()
{
	glfwDestroyWindow(GlWindow);
	GlWindow = nullptr;
}

CGLFWWindow::CGLFWWindow(std::string InWindowName, SVector2i InWindowSize, bool bFullScreen)
: CEngineWindow(InWindowName, InWindowSize, bFullScreen)
{
	// TODO: Only call below once for whole program
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	
	// Enable debugging context
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	// TODO: Only needed once, should be in main glfw api separate to per window?
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
	glfwSetWindowUserPointer(GlWindow, reinterpret_cast<void *>(this));

	if (!bFullScreen)
	{
		glfwSetWindowPos(GlWindow, WindowPosition.X, WindowPosition.Y);
	}

	glfwSetWindowSizeCallback(GlWindow, [](GLFWwindow* window, int width, int height)
	{
		CGLFWWindow* OwningGlfwWindow = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
		OwningGlfwWindow->OnWindowResized(width, height);
	});
	
	glfwSetFramebufferSizeCallback(GlWindow, [](GLFWwindow* window, int width, int height)
	{
		CGLFWWindow* OwningGlfwWindow = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
		OwningGlfwWindow->OnFrameBufferResized(width, height);
	});	
	glfwSetWindowFocusCallback(GlWindow, [](GLFWwindow* window, int focused)
	{
		CGLFWWindow* OwningGlfwWindow = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
		OwningGlfwWindow->OnFocusChanged(focused);
	});
	
	glfwSetKeyCallback(GlWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		CGLFWWindow* OwningGlfwWindow = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
		CInput::KeyEventType EventType = CInput::KeyEventType::Pressed;
		switch (action)
		{
		case GLFW_PRESS:
			EventType  = CInput::KeyEventType::Pressed;
			break;
		case GLFW_REPEAT:
			EventType  = CInput::KeyEventType::Repeat;
			break;
		case GLFW_RELEASE:
			EventType  = CInput::KeyEventType::Released;			
			break;
		}
		// TODO: Convert to custom keycode
		OwningGlfwWindow->KeyPress(key, scancode, EventType, ConvertModiferToCustomInputMod(mods));
	});
	glfwSetCharCallback(GlWindow, [](GLFWwindow* window, unsigned int codepoint)
	{
		CGLFWWindow* OwningGlfwWindow = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
		// TODO: Convert to custom keycode
		OwningGlfwWindow->KeyTyped(codepoint);
	});
	glfwSetCursorPosCallback(GlWindow, [](GLFWwindow* window, double xpos, double ypos)
	{
		CGLFWWindow* OwningGlfwWindow = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
		OwningGlfwWindow->CursorMoved((int)xpos, (int)ypos);
	});
	glfwSetMouseButtonCallback(GlWindow, [](GLFWwindow* window, int button, int action, int mods)
	{
		CGLFWWindow* OwningGlfwWindow = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
		CInput::KeyEventType EventType = CInput::KeyEventType::Pressed;
		switch (action)
		{
		case GLFW_PRESS:
			EventType  = CInput::KeyEventType::Pressed;
			break;
		case GLFW_REPEAT:
			EventType  = CInput::KeyEventType::Repeat;
			break;
		case GLFW_RELEASE:
			EventType  = CInput::KeyEventType::Released;			
			break;
		}
		OwningGlfwWindow->MouseButtonPress(button, EventType, ConvertModiferToCustomInputMod(mods));
	});

	glfwSetScrollCallback(GlWindow, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		CGLFWWindow* OwningGlfwWindow = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
		OwningGlfwWindow->ScrollWheel((float)xoffset, (float)yoffset);
	});
}

void CGLFWWindow::CreateGraphicsInstance()
{
	glfwMakeContextCurrent(GlWindow);
	CEngineWindow::CreateGraphicsInstance();
}

void CGLFWWindow::SetWindowFullScreen(bool bInFullscreen)
{
	// TODO:
}

bool CGLFWWindow::ShouldWindowClose() const
{	
	return glfwWindowShouldClose(GlWindow);
}

void CGLFWWindow::PreRender()
{
	glfwMakeContextCurrent(GlWindow);	
	CEngineWindow::PreRender();
}

void CGLFWWindow::PostRender()
{
	CEngineWindow::PostRender();
	
	glfwPollEvents();
	glfwSwapBuffers(GlWindow);
}

// TODO: VSync with glfwSwapInterval(1);

void CGLFWWindow::FocusWindow() const
{
	glfwFocusWindow(GlWindow);
}
void CGLFWWindow::SetCursorPosition(SVector2i InCursorPosition)
{
	CEngineWindow::SetCursorPosition(InCursorPosition);
	glfwSetCursorPos(GlWindow, InCursorPosition.X, InCursorPosition.Y);
}

bool CGLFWWindow::CloseWindow()
{
	glfwSetWindowShouldClose(GlWindow, true);
	return true;
}

// SVector2i CGLFWWindow::GetWindowPosition()
// {
// 	int x, y;
// 	glfwGetWindowPos(GlWindow, &x, &y);
// 	return {x, y};
// }

void CGLFWWindow::SetWindowPosition(SVector2i InWindowPosition)
{
	glfwSetWindowPos(GlWindow, InWindowPosition.X, InWindowPosition.Y);
}

// SVector2i CGLFWWindow::GetWindowSize()
// {
// 	int WindowWidth, WindowHeight;
// 	glfwGetWindowSize(GlWindow, &WindowWidth, &WindowHeight);
// 	return {WindowWidth, WindowHeight};
// }

void CGLFWWindow::OnWindowResized(int NewWidth, int NewHeight)
{
	CEngineWindow::OnWindowResized(NewWidth, NewHeight);	
}

void CGLFWWindow::OnFocusChanged(bool bIsFocused)
{
	CEngineWindow::OnFocusChanged(bIsFocused);
}

void CGLFWWindow::OnFrameBufferResized(int NewWidth, int NewHeight)
{
}

int CGLFWWindow::ConvertModiferToCustomInputMod(int GlfwMods)
{
	int NewMod = 0;
	if (GlfwMods & GLFW_MOD_SHIFT)
	{
		NewMod |= CInput::ModiferType::Shift;
	}
	if (GlfwMods & GLFW_MOD_ALT)
	{
		NewMod |= CInput::ModiferType::Alt;
	}
	if (GlfwMods & GLFW_MOD_CONTROL)
	{
		NewMod |= CInput::ModiferType::Control;
	}
	return NewMod;
}

void CGLFWWindow::SetCursorVisible(bool bSetVisible)
{
	CEngineWindow::SetCursorVisible(bSetVisible);
	glfwSetInputMode(GlWindow, GLFW_CURSOR, bSetVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void glfw_onError(int error, const char* description)
{
	// print message in Windows popup dialog box
	CLogManager::GetInstance()->DisplayLogError(description);
	MessageBoxA(NULL, description, "GLFW error", MB_OK);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}