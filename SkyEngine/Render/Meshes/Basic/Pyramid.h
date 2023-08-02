// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class ENGINE_API CPyramid : public CMeshComponent
{
public:
	using CMeshComponent::CMeshComponent;
	~CPyramid();
	
protected:
	MeshData GetMeshData() override;
};
