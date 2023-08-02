// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class ENGINE_API CCube : public CMeshComponent
{
public:
	using CMeshComponent::CMeshComponent;
	~CCube();
	
	bool CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos, TPointer<Entity> EntityCheck) override;
protected:
	MeshData GetMeshData() override;
};
