// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Graphics/GraphicsInstance.h"

class GLFWInstance : public IGraphicsInstance
{
public:
	GLFWInstance();
	
	void SwapBuffers(std::shared_ptr<IGraphicsWindow> GraphicsWindow) override;

	void Render(std::shared_ptr<IGraphicsWindow> GraphicsWindow, std::vector<std::shared_ptr<Entity>> Entities, std::vector<std::shared_ptr<UIElement>> UIElements) override;

	void PreRender(std::shared_ptr<IGraphicsWindow> GraphicsWindow) override;
	void PostRender(std::shared_ptr<IGraphicsWindow> GraphicsWindow) override;
	
	void WindowSetup(std::shared_ptr<IGraphicsWindow> InGraphicsWindow) override;
};
