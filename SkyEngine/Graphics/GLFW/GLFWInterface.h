// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Graphics/GraphicsInterface.h"

class GLFWInterface : public IGraphicsInterface
{
public:
	GLFWInterface();
	~GLFWInterface() override;
	std::string GetGraphicsDisplayName() override;
	
	std::shared_ptr<IGraphicsWindow> CreateNewWindow(const std::string& InWindowName, Vector2 InWindowSize, bool bFullScreen) override;
	
};
