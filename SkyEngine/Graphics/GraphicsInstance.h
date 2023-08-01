// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <memory>
#include <vector>

#include "Math/FTransform.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"

class UIElement;
class Entity;
class Mesh;
class IGraphicsWindow;

class IGraphicsInstance
{
public:
	virtual ~IGraphicsInstance() = default;

	virtual void Render(Pointer<IGraphicsWindow> GraphicsWindow, std::vector<Pointer<Entity>> Entities, std::vector<Pointer<UIElement>> UIElements);
	virtual void RenderEntity(Pointer<Entity> Entity);
	virtual void RenderMesh(Pointer<Mesh> Mesh, FTransform Transform) = 0;
	virtual void RenderUIElement(Pointer<UIElement> UserInterfaceItem) = 0;
	virtual void CleanupMesh(Pointer<Mesh> Mesh) = 0;
	
	virtual void PreRender(Pointer<IGraphicsWindow> GraphicsWindow) {}
	virtual void PostRender(Pointer<IGraphicsWindow> GraphicsWindow) {}
	
	template <typename T>
	void PassAttributeToShader(int32_t ShaderLocation, T Attribute);
	
	// TODO: Create colour type
	Vector3 ClearColour = Vector3(0.3f, 0.8f, 0.9f);	

protected:
	virtual void PassAttributeToShader(int32_t ShaderLocation, float Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, int Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, bool Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, Vector3 Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, Vector4 Attribute) = 0;
	virtual void PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute) = 0;
};

template <typename T>
void IGraphicsInstance::PassAttributeToShader(int32_t ShaderLocation, T Attribute)
{
	PassAttributeToShader(ShaderLocation, Attribute);
}
