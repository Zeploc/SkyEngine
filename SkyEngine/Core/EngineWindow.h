// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

#include "Delegates.h"
#include "Math/Vector2.h"

class IGraphicsWindow;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API EngineWindow : public std::enable_shared_from_this<EngineWindow>
{
public:
	EngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen);
	~EngineWindow();
	static TPointer<EngineWindow> CreateEngineWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bInFullScreen = false);

	TPointer<IGraphicsWindow> GetGraphicsWindow() const { return GraphicsWindow; }

	bool ShouldWindowClose() const;
	
	std::string GetWindowName() { return WindowName; }

	SVector2i GetSize();
	
	SVector2i GetPosition();
	void SetWindowPosition(SVector2i InPosition);

	// TODO: Pass in window in delegate
	FMulticastDelegate OnFocusChanged;
	
protected:
	void OnFocusedChanged();
	
	std::string WindowName;
	SVector2i WindowSize;
	SVector2i WindowPosition;
	bool bFullscreen = false;

	TPointer<IGraphicsWindow> GraphicsWindow;
	
};
