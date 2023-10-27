// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "GLIncludes.h"
#include "Graphics/Renderer.h"

class GLRenderer : public IRenderer
{
public:
	GLRenderer();
	
	void ApplyMaterialFlags(TPointer<CMaterialInterface>InMaterial) override;
	
	void RenderMesh(TPointer<CMeshComponent> Mesh, STransform Transform) override;
	void RenderUIElement(TPointer<UIElement> UserInterfaceItem) override;
	void CleanupMesh(TPointer<CMeshComponent> Mesh) override;

	void PreRender() override;
	void PostRender() override;

	// TODO: Resize function

	void StoreMVP(STransform Transform, unsigned int Program);
	void SetWireframeMode(bool bInWireframeEnabled) override;
	void SetRenderViewport(const SVector2i InViewportPosition, const SVector2i InViewportSize) override;

	void RenderImGui() override;
};
