#include "MeshData.h"

#include "Core/Application.h"

MeshData::MeshData(const std::vector<float> &PositionData, const std::vector<uint32_t> &IndexData, const std::vector<float>& NormalData)
{
	Positions = PositionData;
	Normals = NormalData;
	Indices = IndexData;
}

void MeshData::SetUVs(const std::vector<float>& UVData)
{
	UVs = UVData;
}

void MeshData::BindData(unsigned vao) const
{
	std::vector<float> OutVertices;
	std::vector<uint32_t> OutIndices;
	GetFinalData(OutVertices, OutIndices);
	GetGraphicsAPI()->BindArray(OutVertices, OutIndices, vao);	
}

void MeshData::GetFinalData(std::vector<float>& OutVertices, std::vector<uint32_t>& OutIndices) const
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

int MeshData::GetIndicesCount() const
{
	return static_cast<int>(Indices.size());
}

void MeshData::PushPositions(std::vector<float>& OutVertices, int VertexIndex) const
{
	const int StartingPosition = VertexIndex * POSITION_ELEMENTS_COUNT;
	for (int i = 0; i < POSITION_ELEMENTS_COUNT; ++i)
	{
		const int PositionIndex = StartingPosition + i;
		OutVertices.push_back(Positions[PositionIndex]);
	}	
}

void MeshData::PushUVs(std::vector<float>& OutVertices, int VertexIndex) const
{
	const int StartingPosition = VertexIndex * UV_ELEMENTS_COUNT;
	for (int i = 0; i < UV_ELEMENTS_COUNT; ++i)
	{
		const int UVIndex = StartingPosition + i;
		OutVertices.push_back(UVs[UVIndex]);
	}
}

void MeshData::PushNormals(std::vector<float>& OutVertices, int VertexIndex) const
{
	const int StartingPosition = VertexIndex * NORMAL_ELEMENTS_COUNT;
	for (int i = 0; i < NORMAL_ELEMENTS_COUNT; ++i)
	{
		const int NormalIndex = StartingPosition + i;
		OutVertices.push_back(Normals[NormalIndex]);
	}
}
