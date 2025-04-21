#include "SEPCH.h"
#include "MeshManager.h"

#include <format>
#include <memory>

#include "Mesh.h"
#include "Basic/DefaultMeshes.h"
#include "Core/Application.h"
#include "System/LogManager.h"

CMeshManager::CMeshManager()
{
}

void CMeshManager::Init()
{
	DefaultMesh::GetCube();
	DefaultMesh::GetPlane();
	DefaultMesh::GetSphere();
	DefaultMesh::GetPyramid();
	DefaultMesh::GetBox();
	// AddMesh(MESH_CUBE, std::make_shared<CMesh>(new DefaultMesh::GetCubeData());
	// AddMesh(MESH_PLANE, DefaultMesh::GetPlaneData());
	// AddMesh(MESH_SPHERE, DefaultMesh::GetSphereData());
	// AddMesh(MESH_PYRAMID, DefaultMesh::GetPyramidData());
}

bool CMeshManager::AddMesh(const std::string MeshName, const CMesh& MeshData)
{
	// if (Meshes.contains(MeshName))
	// {
	// 	CLogManager::Get()->DisplayError(std::format("Attempted to add mesh {} that already exists", MeshName));
	// 	return false;
	// }
	// if (!MeshData.IsValid())
	// {
	// 	CLogManager::Get()->DisplayError(std::format("Attempted to add mesh {} that has no vertices", MeshName));
	// 	return false;
	// }
	// Meshes.insert(std::make_pair(MeshName, CMesh(MeshData)));
	// CMesh& StoredMesh = Meshes[MeshName];
	// StoredMesh.SetupMeshData();
	// CLogManager::Get()->DisplayMessage(std::format("Created Mesh {} with vao {}", MeshName, StoredMesh.GetVao()));
	return true;
}

bool CMeshManager::HasMesh(const std::string MeshName) const
{
	return Meshes.contains(MeshName);
}

CMesh CMeshManager::GetMesh(const std::string MeshName)
{
	if (!Meshes.contains(MeshName))
	{
		return CMesh();
	}
	return Meshes[MeshName];
}

TArray<std::string> CMeshManager::GetAvailableMeshes() const
{
	TArray<std::string> MeshNames;
	for (auto MeshPair : Meshes)
	{
		MeshNames.push_back(MeshPair.first);
	}
	return MeshNames;
}
