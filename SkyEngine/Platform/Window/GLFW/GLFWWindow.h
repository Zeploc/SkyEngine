// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "GLFWIncludes.h"

#include "Platform/Window/GraphicsWindow.h"

class GLFWWindow : public IGraphicsWindow
{
public:
	~GLFWWindow() override;
	GLFWWindow(std::string InWindowName, SVector2i InWindowSize, bool bFullScreen);
	void CreateGraphicsInstance() override;
	
	void SetWindowFullScreen(bool bFullScreen) override;
	bool ShouldWindowClose() const override;

	void PreRender() override;
	void PostRender() override;
	
	// TODO: Remove once systems moved to interfaces/not glfw specific
	GLFWwindow* GetGlWindow() const { return GlWindow; }

	void FocusWindow() const override;

	void SetCursorPosition(SVector2i InCursorPosition) override;

	bool CloseWindow() override;

	SVector2i GetWindowPosition() override;

	void SetWindowPosition(SVector2i InWindowPosition) override;

	SVector2i GetWindowSize() override;


protected:
	
	GLFWwindow* GlWindow = nullptr;
};
