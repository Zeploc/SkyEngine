// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <memory>
#include <vector>

#include "Math/FTransform.h"
#include "Math/Vector.h"

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
	
	// TODO: Create colour type
	Vector3 ClearColour = Vector3(0.3f, 0.8f, 0.9f);
};
