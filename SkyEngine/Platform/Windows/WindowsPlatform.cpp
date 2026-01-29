// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "WindowsPlatform.h"

#include "FmodAudio.h"
#include "Core/Application.h"
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

TSharedPointer<IAudioInterface> WindowsPlatform::GetAudioInterface()
{
	if (!Audio)
	{
		Audio = std::make_shared<FmodAudio>();
	}
	return Audio;
}

TSharedPointer<CEngineWindow> WindowsPlatform::CreateNewWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bFullScreen)
{
	TSharedPointer<CGLFWWindow> NewWindow = std::make_shared<CGLFWWindow>(InWindowName, InWindowSize, bFullScreen);
	if (!NewWindow->GetGlWindow())
	{
		NewWindow.reset();
	}
	return NewWindow;
}

double WindowsPlatform::GetTime()
{
	return glfwGetTime();
}

int32_t WindowsPlatform::DisplayMessageBox(std::string Title, std::string Message, uint32_t Options)
{
	// TODO: Relevant window (blocking input/focus to other windows)
	return MessageBoxA(NULL, Message.c_str(), Title.c_str(), Options);
}
