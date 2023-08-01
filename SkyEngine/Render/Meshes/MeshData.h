// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <vector>

#include "Math/Vector4.h"

class ENGINE_API MeshData
{
public:
	MeshData(const std::vector<float> &PositionData, const std::vector<uint32_t> &IndexData, const std::vector<float>& NormalData);
	
	void SetUVs(const std::vector<float>& UVData);

	void BindData(unsigned int vao);

	void GetFinalData(std::vector<float>& OutVertices, std::vector<uint32_t>& OutIndices) const;
	int GetIndicesCount() const;

	// TODO: Is this needed here? should be in material
	Vector4 Colour;
	
#define POSITION_ELEMENTS_COUNT 3
#define NORMAL_ELEMENTS_COUNT 3
#define UV_ELEMENTS_COUNT 2
	
protected:
	void PushPositions(std::vector<float>& OutVertices, int VertexIndex) const;
	void PushColour(std::vector<float>& OutVertices, int VertexIndex) const;
	void PushUVs(std::vector<float>& OutVertices, int VertexIndex) const;
	void PushNormals(std::vector<float>& OutVertices, int VertexIndex) const;
	
	std::vector<float> Positions;
	std::vector<uint32_t> Indices;
	std::vector<float> UVs;
	std::vector<float> Normals;
};
