// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class ENGINE_API Pyramid : public Mesh
{
public:
	using Mesh::Mesh;
	~Pyramid();
	
protected:
	MeshData GetMeshData() override;
};
