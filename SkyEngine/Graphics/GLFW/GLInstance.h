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
	void PassAttributeToShader(int32_t ShaderID, float Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, int Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, bool Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, Vector3 Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, Vector4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) override;
};
