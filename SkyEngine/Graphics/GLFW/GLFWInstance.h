// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "GLFWIncludes.h"

#include "Graphics/GraphicsInstance.h"

class GLFWInstance : public IGraphicsInstance
{
public:
	GLFWInstance();
	
	void SwapBuffers(Pointer<IGraphicsWindow> GraphicsWindow) override;

	void RenderMesh(Pointer<Mesh> Mesh, FTransform Transform) override;
	void RenderUIElement(Pointer<UIElement> UserInterfaceItem) override;
	void CleanupMesh(Pointer<Mesh> Mesh) override;

	void PreRender(Pointer<IGraphicsWindow> GraphicsWindow) override;
	void PostRender(Pointer<IGraphicsWindow> GraphicsWindow) override;
	
	void WindowSetup(Pointer<IGraphicsWindow> InGraphicsWindow) override;

protected:
	void StoreMVP(FTransform Transform, unsigned int Program);
};
