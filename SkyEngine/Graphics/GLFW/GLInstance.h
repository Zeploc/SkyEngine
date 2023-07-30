// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <glew/glew.h>
#include <gl/GL.h>

#include "Graphics/GraphicsInstance.h"

class GLInstance : public IGraphicsInstance
{
public:
	GLInstance();
	
	void RenderMesh(Pointer<Mesh> Mesh, FTransform Transform) override;
	void RenderUIElement(Pointer<UIElement> UserInterfaceItem) override;
	void CleanupMesh(Pointer<Mesh> Mesh) override;

	void PreRender(Pointer<IGraphicsWindow> GraphicsWindow) override;
	void PostRender(Pointer<IGraphicsWindow> GraphicsWindow) override;

protected:
	void StoreMVP(FTransform Transform, unsigned int Program);
};
