// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "../Mesh.h"

#define MESH_CUBE "Cube"
#define MESH_PLANE "Plane"
#define MESH_SPHERE "Sphere"
#define MESH_PYRAMID "Pyramid"

namespace DefaultMesh
{
	ENGINE_API TAssetObjectPointer<CMesh> GetOrCreateMeshAsset(std::string MeshName, const std::vector<float> VertexPositions, const std::vector<float> Normals, const std::vector<uint32_t> Indices, const std::vector<float> UVs = {});
	
	ENGINE_API TAssetObjectPointer<CMesh> GetCube();
	ENGINE_API TAssetObjectPointer<CMesh> GetPlane();
	ENGINE_API TAssetObjectPointer<CMesh> GetSphere();
	ENGINE_API TAssetObjectPointer<CMesh> GetPyramid();
	 
	ENGINE_API TAssetObjectPointer<CMesh> GetBox();
}
