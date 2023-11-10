// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class CMeshData;

#define MESH_CUBE "Cube"
#define MESH_PLANE "Plane"
#define MESH_SPHERE "Sphere"
#define MESH_PYRAMID "Pyramid"

// TODO: Bind mesh when added and store in mesh data?

class ENGINE_API CMeshManager
{
public:
	CMeshManager();
	void Init();
	
	bool AddMesh(const std::string MeshName, const CMeshData& MeshData);
	bool HasMesh(const std::string MeshName) const;
	CMeshData GetMesh(const std::string MeshName);
	
private:
	std::map<std::string, CMeshData> MeshDatas;
};

