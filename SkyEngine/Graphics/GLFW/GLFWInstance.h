// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Graphics/GraphicsInstance.h"

class GLFWInstance : public IGraphicsInstance
{
public:
	GLFWInstance();
	
	void SwapBuffers(Pointer<IGraphicsWindow> GraphicsWindow) override;

	void Render(Pointer<IGraphicsWindow> GraphicsWindow, std::vector<Pointer<Entity>> Entities, std::vector<Pointer<UIElement>> UIElements) override;

	void PreRender(Pointer<IGraphicsWindow> GraphicsWindow) override;
	void PostRender(Pointer<IGraphicsWindow> GraphicsWindow) override;
	
	void WindowSetup(Pointer<IGraphicsWindow> InGraphicsWindow) override;
};
