// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Cube.h"

// Engine Includes //
#include "DefaultMeshes.h"
#include "Render/Shaders/ShaderManager.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Shaders/PBRShader.h"

CCube::CCube(const TSharedPointer<Entity>& InOwner, const TSharedPointer<CMaterialInterface>& InMaterial)
: CMeshComponent(InOwner, DefaultMesh::GetCube(), InMaterial)
{
}

CCube::~CCube()
{
}