// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

#include "Delegates.h"
#include <string>
#include "Math/Vector2.h"

class IGraphicsWindow;

class ENGINE_API EngineWindow : public std::enable_shared_from_this<EngineWindow>
{
public:
	EngineWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bInFullScreen);
	~EngineWindow();
	static Pointer<EngineWindow> CreateEngineWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bInFullScreen = false);

	Pointer<IGraphicsWindow> GetGraphicsWindow() const { return GraphicsWindow; }

	bool ShouldWindowClose() const;
	
	std::string GetWindowName() { return WindowName; }

	Vector2 GetSize();
	
	Vector2 GetPosition();
	void SetWindowPosition(Vector2 InPosition);

	// TODO: Pass in window in delegate
	FMulticastDelegate OnFocusChanged;
	
protected:
	void OnFocusedChanged();
	
	std::string WindowName;
	Vector2 WindowSize;
	Vector2 WindowPosition;
	bool bFullscreen = false;

	Pointer<IGraphicsWindow> GraphicsWindow;
	
};
