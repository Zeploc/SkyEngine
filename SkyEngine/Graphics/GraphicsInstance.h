// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <map>
#include <memory>
#include <vector>

#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"

class UIElement;
class Entity;
class CMeshComponent;
class IGraphicsWindow;
class CShader;
class CMaterial;
class CTexture;

class IGraphicsInstance : public std::enable_shared_from_this<IGraphicsInstance>
{
public:
	virtual ~IGraphicsInstance() = default;

	void InsertEntityMeshToRenderList(std::map<TPointer<CMaterial>, std::vector<TPointer<CMeshComponent>>> MeshesByMaterial, const TPointer<Entity>& EntityToRender);
	virtual void Render(TPointer<IGraphicsWindow> GraphicsWindow, std::vector<TPointer<Entity>> Entities, std::vector<TPointer<UIElement>> UIElements);
	virtual void RenderEntity(TPointer<Entity> Entity);
	virtual void RenderMesh(TPointer<CMeshComponent> Mesh, STransform Transform) = 0;
	virtual void RenderUIElement(TPointer<UIElement> UserInterfaceItem) = 0;
	virtual void CleanupMesh(TPointer<CMeshComponent> Mesh) = 0;
	virtual void BindShader(uint32_t ShaderProgramID) = 0;
	virtual void ApplyMaterialFlags(TPointer<CMaterial> InMaterial) = 0;
	// TODO: Handle multiple
	virtual void BindTexture(TPointer<CTexture> InTexture) = 0;
	
	virtual void PreRender(TPointer<IGraphicsWindow> GraphicsWindow) {}
	virtual void PostRender(TPointer<IGraphicsWindow> GraphicsWindow) {}
	
	template <typename T>
	void PassAttributeToShader(int32_t ShaderLocation, T Attribute);
	
	// TODO: Create colour type
	SVector ClearColour = SVector(0.3f, 0.8f, 0.9f);
	TPointer<CShader> ActiveShader;

protected:
	virtual void InternalPassAttributeToShader(int32_t ShaderLocation, float Attribute) = 0;
	virtual void InternalPassAttributeToShader(int32_t ShaderLocation, int Attribute) = 0;
	virtual void InternalPassAttributeToShader(int32_t ShaderLocation, bool Attribute) = 0;
	virtual void InternalPassAttributeToShader(int32_t ShaderLocation, SVector Attribute) = 0;
	virtual void InternalPassAttributeToShader(int32_t ShaderLocation, SVector4 Attribute) = 0;
	virtual void InternalPassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) = 0;
};

template <typename T>
void IGraphicsInstance::PassAttributeToShader(int32_t ShaderLocation, T Attribute)
{
	InternalPassAttributeToShader(ShaderLocation, Attribute);
}
