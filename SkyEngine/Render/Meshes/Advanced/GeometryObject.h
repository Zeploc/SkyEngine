// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class ENGINE_API GeometryObject : public Mesh
{
public:
	GeometryObject(glm::vec4 Colour);

	~GeometryObject();

	void BindGeometryObject();

	void Rebind() override;

	// TODO: Update to graphics instance
	void Render(FTransform Newtransform);
protected:
	MeshData GetMeshData() override;
};
