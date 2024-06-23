// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Object.h"
#include "Math/Transform.h"
#include "Math/Vector4.h"
#include "Math/Vector.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

struct STriangle
{
	SVector Position1;
	SVector Position2;
	SVector Position3;

	void TransformTriangle(STransform Transform);
	
	/* Triangle should be in world space */
	bool TestHit(SVector RayStart, SVector RayDirection, SVector& HitPos) const;
};

class ENGINE_API CMeshData : public CAssetObject
{
public:
	CMeshData();
	CMeshData(const TArray<float> &PositionData, const TArray<uint32_t> &IndexData, const TArray<float>& NormalData);
	~CMeshData() {}
	
	void SetUVs(const std::vector<float>& UVData);

	void SetupMeshData();

	void GetFinalData(TArray<float>& OutVertices, TArray<uint32_t>& OutIndices) const;
	bool IsValid() const { return GetIndicesCount() > 0; }
	int GetIndicesCount() const;
	uint32_t GetVao() const { return Vao; }
	bool HasUVData() const { return !UVs.empty(); }

	TArray<float> GetPositions() const { return Positions; }
	TArray<uint32_t> GetIndices() const { return Indices; }

	TArray<STriangle> GetTriangles() const;

	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
	std::string GetAssetClassName() override;
	static std::string GetStaticName(); 
	void Open() override;
	
#define POSITION_ELEMENTS_COUNT 3
#define NORMAL_ELEMENTS_COUNT 3
#define UV_ELEMENTS_COUNT 2
	
protected:
	void BindData(unsigned int vao) const;
	void PushPositions(TArray<float>& OutVertices, int VertexIndex) const;
	void PushUVs(TArray<float>& OutVertices, int VertexIndex) const;
	void PushNormals(TArray<float>& OutVertices, int VertexIndex) const;

protected:
	TArray<float> Positions;
	TArray<uint32_t> Indices;
	TArray<float> UVs;
	TArray<float> Normals;

	uint32_t Vao = 0;
	int IndicesCount;
};
