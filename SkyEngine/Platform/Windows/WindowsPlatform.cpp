// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
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

TPointer<CEngineWindow> WindowsPlatform::CreateNewWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bFullScreen)
{
	TPointer<CGLFWWindow> NewWindow = std::make_shared<CGLFWWindow>(InWindowName, InWindowSize, bFullScreen);
	if (!NewWindow->GetGlWindow())
	{
		NewWindow.reset();
	}
	return NewWindow;
}
