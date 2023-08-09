// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "GLFWIncludes.h"
#include "Platform/Window/EngineWindow.h"

class CGLFWWindow : public CEngineWindow
{
public:
	CGLFWWindow(std::string InWindowName, SVector2i InWindowSize, bool bFullScreen);
	~CGLFWWindow() override;
	void CreateGraphicsInstance() override;
	
	void SetWindowFullScreen(bool bInFullScreen) override;
	bool ShouldWindowClose() const override;
	bool CloseWindow() override;
	void FocusWindow() const override;
	
	void PreRender() override;
	void PostRender() override;
	
	// TODO: Remove once systems moved to interfaces/not glfw specific
	GLFWwindow* GetGlWindow() const { return GlWindow; }

	void SetCursorPosition(SVector2i InCursorPosition) override;
	void SetWindowPosition(SVector2i InWindowPosition) override;
	void SetCursorVisible(bool bSetVisible) override;


protected:
	void OnWindowResized(int NewWidth, int NewHeight) override;
	void OnFocusChanged(bool bIsFocused) override;
	void OnFrameBufferResized(int NewWidth, int NewHeight) override;

	static int ConvertModiferToCustomInputMod(int GlfwMods);
	
	GLFWwindow* GlWindow = nullptr;
};
