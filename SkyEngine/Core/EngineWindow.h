// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

#include "Delegates.h"
#include "Math/Vector2.h"

class IGraphicsWindow;

class ENGINE_API EngineWindow : public std::enable_shared_from_this<EngineWindow>
{
public:
	EngineWindow(const std::string& InWindowName, SVector2 InWindowSize, bool bInFullScreen);
	~EngineWindow();
	static TPointer<EngineWindow> CreateEngineWindow(const std::string& InWindowName, SVector2 InWindowSize, bool bInFullScreen = false);

	TPointer<IGraphicsWindow> GetGraphicsWindow() const { return GraphicsWindow; }

	bool ShouldWindowClose() const;
	
	std::string GetWindowName() { return WindowName; }

	SVector2 GetSize();
	
	SVector2 GetPosition();
	void SetWindowPosition(SVector2 InPosition);

	// TODO: Pass in window in delegate
	FMulticastDelegate OnFocusChanged;
	
protected:
	void OnFocusedChanged();
	
	std::string WindowName;
	SVector2 WindowSize;
	SVector2 WindowPosition;
	bool bFullscreen = false;

	TPointer<IGraphicsWindow> GraphicsWindow;
	
};
