// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Triangle.h"

// Engine Includes //
#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"

CTriangle::CTriangle(const TPointer<Entity>& InOwner, glm::vec3 _Point1, glm::vec3 _Point2, glm::vec3 _Point3, TPointer<CMaterial> InMaterial)
: CMeshComponent(InOwner, 0.0f, 0.0f, 0.0f, InMaterial)
{
	Point1 = _Point1;
	Point2 = _Point2;
	Point3 = _Point3;
	glm::vec3 CenterPoint;
	CenterPoint.x = (Point1.x + Point2.x + Point3.x) / 3;
	CenterPoint.y = (Point1.y + Point2.y + Point3.y) / 3;
	CenterPoint.z = (Point1.z + Point2.z + Point3.z) / 3;
	m_fWidth = length(CenterPoint - Point1);
	m_fHeight = m_fWidth;
	BindMeshData();
}

CTriangle::CTriangle(const TPointer<Entity>& InOwner, glm::vec3 CenterPoint, float Width, TPointer<CMaterial> InMaterial)
: CMeshComponent(InOwner, Width, Width, 0.0f, InMaterial)
{
	Point1 = CenterPoint + glm::vec3(0, Width, 0);
	Point2 = CenterPoint + glm::vec3(1, -0.5, 0) * Width;
	Point3 = CenterPoint + glm::vec3(-1, -0.5, 0) * Width;
	BindMeshData();
}

CTriangle::~CTriangle()
{
}

MeshData CTriangle::GetMeshData()
{
	const std::vector<float> VertexPositions = {
		// Front Face
		Point1.x, Point1.y, Point1.z, 
		Point2.x, Point2.y, Point2.z, 
		Point3.x, Point3.y, Point3.z,
	};
	const std::vector<float> Normals = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,	
	};	
	const std::vector<uint32_t> Indices = {
		0, 1, 2, // Triangle
	};

	MeshData TriangleMeshData(VertexPositions, Indices, Normals);

	// TODO: Triangle doesn't support texture/UVs
	assert(!MeshMaterial->HasTexture());
	if (MeshMaterial->HasTexture())
	{
		// PlaneMeshData.SetUVs(UVCoords);
	}
	return TriangleMeshData;
}
