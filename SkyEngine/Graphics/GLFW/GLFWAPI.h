// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Graphics/GraphicsAPI.h"

class IGLFWAPI : public IGraphicsAPI
{
public:
	IGLFWAPI();
	~IGLFWAPI() override;
	std::string GetGraphicsDisplayName() override;
	
	Pointer<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen) override;
	
};
