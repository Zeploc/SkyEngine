// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Math/Vector2.h"

class CEngineWindow;
class UIElement;
class Entity;
class CMeshComponent;
class CShader;
class CMaterialInterface;
class CTexture;

class IGraphicsInstance : public std::enable_shared_from_this<IGraphicsInstance>
{
public:
	virtual ~IGraphicsInstance() = default;

	void InsertEntityMeshToRenderList(std::map<TPointer<CMaterialInterface>, std::vector<TPointer<CMeshComponent>>>& MeshesByMaterial, const TPointer<Entity>& EntityToRender);
	virtual void Render(TPointer<CEngineWindow> GraphicsWindow, std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements);
	virtual void RenderMesh(TPointer<CMeshComponent> Mesh, STransform Transform) = 0;
	virtual void RenderUIElement(TPointer<UIElement> UserInterfaceItem) = 0;
	virtual void CleanupMesh(TPointer<CMeshComponent> Mesh) = 0;
	virtual void BindShader(uint32_t ShaderProgramID) = 0;
	virtual void ApplyMaterialFlags(TPointer<CMaterialInterface> InMaterial) = 0;
	
	virtual void PreRender(TPointer<CEngineWindow> GraphicsWindow) {}
	virtual void PostRender(TPointer<CEngineWindow> GraphicsWindow) {}
	
	virtual void SetWireframeMode(bool bInWireframeEnabled) = 0;
	virtual void SetRenderViewport(const SVector2i InViewportPosition, const SVector2i InViewportSize) = 0;
	virtual void RenderImGui() = 0;
	
	// template <typename T>
	// void PassAttributeToShader(int32_t ShaderLocation, T Attribute);
	
	// TODO: Create colour type
	SVector ClearColour = SVector(0.3f, 0.8f, 0.9f);
	TPointer<CShader> ActiveShader;

	virtual void PassAttributeToShader(int32_t ShaderLocation, float Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, int Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, bool Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, SVector Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, SVector4 Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, TPointer<CTexture> Attribute) = 0;
protected:
};

// template <typename T>
// void IGraphicsInstance::PassAttributeToShader(int32_t ShaderLocation, T Attribute)
// {
// 	PassAttributeToShader(ShaderLocation, Attribute);
// }
