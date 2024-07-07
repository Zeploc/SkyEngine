// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Sphere.h"

// Engine Includes //
#include "../MeshManager.h"
#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "Render/Materials/InternalMaterial.h"

CSphere::CSphere(const THardPointer<Entity>& InOwner, const THardPointer<CMaterialInterface>& InMaterial)
: CMeshComponent(InOwner, MESH_SPHERE, InMaterial)
{
}

CSphere::~CSphere()
{
}