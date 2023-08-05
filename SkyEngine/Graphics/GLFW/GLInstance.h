// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Graphics/GraphicsInstance.h"

class GLInstance : public IGraphicsInstance
{
public:
	GLInstance();
	
	void BindShader(uint32_t ShaderProgramID) override;
	void ApplyMaterialFlags(TPointer<CMaterialInterface>InMaterial) override;
	
	void RenderMesh(TPointer<CMeshComponent> Mesh, STransform Transform) override;
	void RenderUIElement(TPointer<UIElement> UserInterfaceItem) override;
	void CleanupMesh(TPointer<CMeshComponent> Mesh) override;


	void PreRender(TPointer<IGraphicsWindow> GraphicsWindow) override;
	void PostRender(TPointer<IGraphicsWindow> GraphicsWindow) override;

	void StoreMVP(STransform Transform, unsigned int Program);
	void PassAttributeToShader(int32_t ShaderID, float Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, int Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, bool Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, SVector Attribute) override;
	void PassAttributeToShader(int32_t ShaderID, SVector4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) override;
	void PassAttributeToShader(int32_t ShaderLocation, TPointer<CTexture> Attribute) override;
};
