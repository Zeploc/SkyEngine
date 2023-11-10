// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "MeshData.h"

#include "Core/Application.h"
#include "System/LogManager.h"

void STriangle::TransformTriangle(STransform Transform)
{
	Position1 = Transform.TransformPosition(Position1);
	Position2 = Transform.TransformPosition(Position2);
	Position3 = Transform.TransformPosition(Position3);
}

bool STriangle::TestHit(SVector RayStart, SVector RayDirection, SVector& HitPos) const
{
	// TODO: Hit pos
	// Temp:
	HitPos = Position1;
	
	SVector E1 = Position2-Position1;
	SVector E2 = Position3-Position1;
	SVector N = cross(E1,E2);
	float Det = -RayDirection.Dot(N);
	float Invdet = 1.0f/Det;
	SVector AO  = RayStart - Position1;
	SVector DAO = cross(AO, RayDirection);
	float u =  E2.Dot(DAO) * Invdet;
	float v = -E1.Dot(DAO) * Invdet;
	float t =  AO.Dot(N)  * Invdet; 
	return (Det >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u+v) <= 1.0);
}

CMeshData::CMeshData()
{
}

CMeshData::CMeshData(const TArray<float> &PositionData, const TArray<uint32_t> &IndexData, const TArray<float>& NormalData)
	: Positions(PositionData), Indices(IndexData), Normals(NormalData)
{
}

void CMeshData::SetUVs(const TArray<float>& UVData)
{
	UVs = UVData;
}

void CMeshData::BindData(unsigned vao) const
{
	std::vector<float> OutVertices;
	std::vector<uint32_t> OutIndices;
	GetFinalData(OutVertices, OutIndices);
	GetGraphicsAPI()->BindVertexArray(OutVertices, OutIndices, vao);	
}

void CMeshData::SetupMeshData()
{
	Vao = GetGraphicsAPI()->CreateVertexBuffer(*this);
	IndicesCount = GetIndicesCount();
	BindData(Vao);
}

void CMeshData::GetFinalData(TArray<float>& OutVertices, TArray<uint32_t>& OutIndices) const
{
	OutIndices = Indices;
	OutVertices.clear();

	const int NumberOfPositionVertices = static_cast<int>(Positions.size() / POSITION_ELEMENTS_COUNT);
	const int NumberOfNormalVertices = static_cast<int>(Normals.size() / NORMAL_ELEMENTS_COUNT);
	ensure(NumberOfPositionVertices == NumberOfNormalVertices, "Not matching vertex count for mesh data!");
	const bool bHasUVs = !UVs.empty();
	if (bHasUVs)
	{
		const int NumberOfUVVertices = static_cast<int>(UVs.size() / UV_ELEMENTS_COUNT);
		ensure(NumberOfUVVertices == NumberOfPositionVertices, "Not matching vertex count for mesh data!");
	}
	const int NumberOfElements = NumberOfPositionVertices;
	for (int i = 0; i < NumberOfElements; ++i)
	{
		PushPositions(OutVertices, i);
		PushNormals(OutVertices, i);
		if (bHasUVs)
		{
			PushUVs(OutVertices, i);
		}
	}
}

int CMeshData::GetIndicesCount() const
{
	return static_cast<int>(Indices.size());
}

TArray<STriangle> CMeshData::GetTriangles() const
{
	TArray<STriangle> Triangles;
	int CurrentIndex = 0;
	STriangle CurrentTriangle;
	for (uint32_t VertexIndex : Indices)
	{
		uint32_t PositionStartingIndex = VertexIndex * POSITION_ELEMENTS_COUNT;
		float X = Positions[PositionStartingIndex];
		float Y = Positions[PositionStartingIndex + 1];
		float Z = Positions[PositionStartingIndex + 2];
		SVector NewPosition = {X, Y, Z};
		switch (CurrentIndex)
		{
		case 0:
			CurrentTriangle.Position1 = NewPosition;
			CurrentIndex++;
			break;
		case 1:
			CurrentTriangle.Position2 = NewPosition;
			CurrentIndex++;
			break;
		case 2:
			CurrentTriangle.Position3 = NewPosition;
			Triangles.push_back(CurrentTriangle);
			CurrentTriangle = STriangle();
			CurrentIndex = 0;
		}
	}
	return Triangles;
}

void CMeshData::PushPositions(TArray<float>& OutVertices, int VertexIndex) const
{
	const int StartingPosition = VertexIndex * POSITION_ELEMENTS_COUNT;
	for (int i = 0; i < POSITION_ELEMENTS_COUNT; ++i)
	{
		const int PositionIndex = StartingPosition + i;
		OutVertices.push_back(Positions[PositionIndex]);
	}	
}

void CMeshData::PushUVs(TArray<float>& OutVertices, int VertexIndex) const
{
	const int StartingPosition = VertexIndex * UV_ELEMENTS_COUNT;
	for (int i = 0; i < UV_ELEMENTS_COUNT; ++i)
	{
		const int UVIndex = StartingPosition + i;
		OutVertices.push_back(UVs[UVIndex]);
	}
}

void CMeshData::PushNormals(TArray<float>& OutVertices, int VertexIndex) const
{
	const int StartingPosition = VertexIndex * NORMAL_ELEMENTS_COUNT;
	for (int i = 0; i < NORMAL_ELEMENTS_COUNT; ++i)
	{
		const int NormalIndex = StartingPosition + i;
		OutVertices.push_back(Normals[NormalIndex]);
	}
}
