// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "GLFWIncludes.h"

#include "Platform/Window/GraphicsWindow.h"

class GLFWWindow : public IGraphicsWindow
{
public:
	~GLFWWindow() override;
	GLFWWindow(std::string InWindowName, SVector2 InWindowSize, bool bFullScreen);
	void CreateGraphicsInstance() override;
	
	void SetWindowFullScreen(bool bFullScreen) override;
	bool ShouldWindowClose() const override;

	void PreRender() override;
	void PostRender() override;
	
	// TODO: Remove once systems moved to interfaces/not glfw specific
	GLFWwindow* GetGlWindow() const { return GlWindow; }

	void FocusWindow() const override;

	void SetCursorPosition(SVector2 InCursorPosition) override;

	bool CloseWindow() override;

	SVector2 GetWindowPosition() override;

	void SetWindowPosition(SVector2 InWindowPosition) override;

	SVector2 GetWindowSize() override;


protected:
	
	GLFWwindow* GlWindow = nullptr;
};
