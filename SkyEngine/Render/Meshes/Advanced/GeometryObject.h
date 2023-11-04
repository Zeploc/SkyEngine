// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/MeshComponent.h"

class ENGINE_API GeometryObject : public CMeshComponent
{
public:
	GeometryObject(const TPointer<Entity>& InOwner, glm::vec4 Colour);

	~GeometryObject();

	void BindGeometryObject();

	void Rebind() override;

	// TODO: Update to graphics instance
	void Render(STransform Newtransform);
protected:
	MeshData GetMeshData() override;
};
