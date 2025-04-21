// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class CMesh;

// TODO: Bind mesh when added and store in mesh data?

class ENGINE_API CMeshManager
{
public:
	CMeshManager();
	void Init();

	// TODO: Should take mesh asset instead
	bool AddMesh(const std::string MeshName, const CMesh& MeshData);
	bool HasMesh(const std::string MeshName) const;
	CMesh GetMesh(const std::string MeshName);

	TArray<std::string> GetAvailableMeshes() const;
	
private:
	std::map<std::string, CMesh> Meshes;
};

