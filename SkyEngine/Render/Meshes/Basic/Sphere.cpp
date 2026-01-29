// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Sphere.h"

// Engine Includes //
#include "DefaultMeshes.h"
#include "Render/Shaders/ShaderManager.h"
#include "Entity/Entity.h"
#include "Render/Materials/InternalMaterial.h"

CSphere::CSphere(const TSharedPointer<Entity>& InOwner, const TSharedPointer<CMaterialInterface>& InMaterial)
: CMeshComponent(InOwner, DefaultMesh::GetSphere(), InMaterial)
{
}

CSphere::~CSphere()
{
}