#include "SEPCH.h"
#include "MeshManager.h"

#include <format>

#include "MeshData.h"
#include "Basic/DefaultMeshes.h"
#include "System/LogManager.h"

CMeshManager::CMeshManager()
{
}

void CMeshManager::Init()
{
	AddMesh(MESH_CUBE, DefaultMesh::GetCubeData());
	AddMesh(MESH_PLANE, DefaultMesh::GetPlaneData());
	AddMesh(MESH_SPHERE, DefaultMesh::GetSphereData());
	AddMesh(MESH_PYRAMID, DefaultMesh::GetPyramidData());
}

bool CMeshManager::AddMesh(const std::string MeshName, const CMeshData& MeshData)
{
	if (MeshDatas.contains(MeshName))
	{
		CLogManager::Get()->DisplayError(std::format("Attempted to add mesh {} that already exists", MeshName));
		return false;
	}
	if (!MeshData.IsValid())
	{
		CLogManager::Get()->DisplayError(std::format("Attempted to add mesh {} that has no vertices", MeshName));
		return false;
	}
	MeshDatas.insert(std::make_pair(MeshName, CMeshData(MeshData)));
	CMeshData& StoredMesh = MeshDatas[MeshName];
	StoredMesh.SetupMeshData();
	CLogManager::Get()->DisplayMessage(std::format("Created Mesh {} with vao {}", MeshName, StoredMesh.GetVao()));
	return true;
}

bool CMeshManager::HasMesh(const std::string MeshName) const
{
	return MeshDatas.contains(MeshName);
}

CMeshData CMeshManager::GetMesh(const std::string MeshName)
{
	if (!MeshDatas.contains(MeshName))
	{
		return CMeshData();
	}
	return MeshDatas[MeshName];
}

TArray<std::string> CMeshManager::GetAvailableMeshes() const
{
	TArray<std::string> MeshNames;
	for (auto MeshPair : MeshDatas)
	{
		MeshNames.push_back(MeshPair.first);
	}
	return MeshNames;
}
