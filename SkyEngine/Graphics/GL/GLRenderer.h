// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "GLIncludes.h"
#include "Render/Renderer.h"

// TODO: Remove/abstract
class GLRenderer : public CRenderer
{
public:
	GLRenderer();

	void ApplyMaterialFlags(TPointer<CMaterialInterface>InMaterial) override;
	
	void RenderMesh(TPointer<CMeshComponent> Mesh, STransform Transform) override;
	void RenderUIElement(TPointer<UIElement> UserInterfaceItem) override;
	void CleanupMesh(TPointer<CMeshComponent> Mesh) override;

	void StoreMVP(STransform Transform, unsigned int Program);
	void SetWireframeMode(bool bInWireframeEnabled) override;

	void RenderImGui() override;
};
