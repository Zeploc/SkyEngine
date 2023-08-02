// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Graphics/GraphicsInstance.h"

class GLInstance : public IGraphicsInstance
{
public:
	GLInstance();
	
	void BindShader(uint32_t ShaderProgramID) override;
	void ApplyMaterialFlags(TPointer<CMaterial>InMaterial) override;
	void BindTexture(TPointer<CTexture> InTexture) override;
	
	void RenderMesh(TPointer<CMeshComponent> Mesh, STransform Transform) override;
	void RenderUIElement(TPointer<UIElement> UserInterfaceItem) override;
	void CleanupMesh(TPointer<CMeshComponent> Mesh) override;


	void PreRender(TPointer<IGraphicsWindow> GraphicsWindow) override;
	void PostRender(TPointer<IGraphicsWindow> GraphicsWindow) override;

protected:
	void StoreMVP(STransform Transform, unsigned int Program);
	void InternalPassAttributeToShader(int32_t ShaderID, float Attribute) override;
	void InternalPassAttributeToShader(int32_t ShaderID, int Attribute) override;
	void InternalPassAttributeToShader(int32_t ShaderID, bool Attribute) override;
	void InternalPassAttributeToShader(int32_t ShaderID, SVector Attribute) override;
	void InternalPassAttributeToShader(int32_t ShaderID, SVector4 Attribute) override;
	void InternalPassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) override;
};
