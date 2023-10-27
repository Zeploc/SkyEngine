// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Math/Vector2.h"

class IFramebuffer;
class CEngineWindow;
class UIElement;
class Entity;
class CMeshComponent;
class CShader;
class CMaterialInterface;
class CTexture;

// TODO: MOVE TO GRAPHICS API
class IRenderer : public std::enable_shared_from_this<IRenderer>
{
public:
	IRenderer();
	virtual ~IRenderer() = default;	
	
	void InsertEntityMeshToRenderList(std::map<TPointer<CMaterialInterface>, std::vector<TPointer<CMeshComponent>>>& MeshesByMaterial, const TPointer<Entity>& EntityToRender);
	virtual void Render(std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements);
	virtual void RenderMesh(TPointer<CMeshComponent> Mesh, STransform Transform) = 0;
	virtual void RenderUIElement(TPointer<UIElement> UserInterfaceItem) = 0;
	virtual void CleanupMesh(TPointer<CMeshComponent> Mesh) = 0;
	virtual void ApplyMaterialFlags(TPointer<CMaterialInterface> InMaterial) = 0;

	// TODO: Remove passing window
	virtual void PreRender() {}
	virtual void PostRender() {}
	
	// TODO: Resize function
	
	virtual void SetWireframeMode(bool bInWireframeEnabled) = 0;
	/* Where Y=0 is the top of the screen */
	virtual void SetRenderViewport(const SVector2i InViewportPosition, const SVector2i InViewportSize) = 0;
	virtual void RenderImGui() = 0;

	TPointer<IFramebuffer> GetFramebuffer() const { return Framebuffer; }
	
	TPointer<CShader> ActiveShader;
	
	// TODO: Create colour type
	SVector ClearColour = SVector(0.3f, 0.8f, 0.9f);
protected:
	// TODO: Multiple frame buffers (multiple windows, etc)
	TPointer<IFramebuffer> Framebuffer;
	SVector2ui InstanceSize;
};

// template <typename T>
// void IRenderer::PassAttributeToShader(int32_t ShaderLocation, T Attribute)
// {
// 	PassAttributeToShader(ShaderLocation, Attribute);
// }
