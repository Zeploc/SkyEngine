// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Cube.h"

// Engine Includes //
#include "../MeshManager.h"
#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Shaders/PBRShader.h"

CCube::CCube(const THardPointer<Entity>& InOwner, const THardPointer<CMaterialInterface>& InMaterial)
: CMeshComponent(InOwner, MESH_CUBE, InMaterial)
{
}

CCube::~CCube()
{
}