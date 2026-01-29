// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/MeshComponent.h"

class ENGINE_API CSphere : public CMeshComponent
{
public:
	CSphere(const TSharedPointer<Entity>& InOwner, const TSharedPointer<CMaterialInterface>& InMaterial = nullptr);	
	~CSphere();
	
};
