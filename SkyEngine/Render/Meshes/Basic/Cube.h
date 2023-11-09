// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/MeshComponent.h"

class ENGINE_API CCube : public CMeshComponent
{
public:
	CCube(const TPointer<Entity>& InOwner, const TPointer<CMaterialInterface>& InMaterial = nullptr);
	~CCube();
};
