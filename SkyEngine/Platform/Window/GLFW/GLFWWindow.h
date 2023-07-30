// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include "Platform/Window/GraphicsWindow.h"

class GLFWWindow : public IGraphicsWindow
{
public:
	~GLFWWindow() override;
	GLFWWindow(std::string InWindowName, Vector2 InWindowSize, bool bFullScreen);
	void CreateGraphicsInstance() override;
	
	void SetWindowFullScreen(bool bFullScreen) override;
	bool ShouldWindowClose() const override;

	void PreRender() override;
	void PostRender() override;
	
	// TODO: Remove once systems moved to interfaces/not glfw specific
	GLFWwindow* GetGlWindow() const { return GlWindow; }

	void FocusWindow() const override;

	void SetCursorPosition(Vector2 InCursorPosition) override;

	bool CloseWindow() override;

	Vector2 GetWindowPosition() override;

	void SetWindowPosition(Vector2 InWindowPosition) override;

	Vector2 GetWindowSize() override;


protected:
	
	GLFWwindow* GlWindow = nullptr;
};
