// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class ENGINE_API Cube : public Mesh
{
public:
	using Mesh::Mesh;
	~Cube();
	
	bool CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, Pointer<Entity> EntityCheck) override;
protected:
	MeshData GetMeshData() override;
};
