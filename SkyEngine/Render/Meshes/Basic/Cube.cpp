// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Cube.h"

// Engine Includes //
#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Shaders/PBRShader.h"

CCube::~CCube()
{
}

MeshData CCube::GetMeshData()
{
	const float HalfWidth = m_fWidth / 2;
	const float HalfHeight = m_fHeight / 2;
	const float HalfDepth = m_fDepth / 2;

	const std::vector<float> VertexPositions = {
		// Front Face
		-HalfWidth, HalfHeight, HalfDepth, 
		HalfWidth, HalfHeight, HalfDepth, 
		HalfWidth, -HalfHeight, HalfDepth,
		-HalfWidth, -HalfHeight, HalfDepth,
		// Right Face
		HalfWidth, HalfHeight, HalfDepth, 
		HalfWidth, HalfHeight, -HalfDepth,
		HalfWidth, -HalfHeight, -HalfDepth,
		HalfWidth, -HalfHeight, HalfDepth,
		// Back Face
		HalfWidth, HalfHeight, -HalfDepth, 
		-HalfWidth, HalfHeight, -HalfDepth,
		-HalfWidth, -HalfHeight, -HalfDepth,
		HalfWidth, -HalfHeight, -HalfDepth,
		// Left Face
		-HalfWidth, HalfHeight, -HalfDepth,
		-HalfWidth, HalfHeight, HalfDepth, 
		-HalfWidth, -HalfHeight, HalfDepth,
		-HalfWidth, -HalfHeight, -HalfDepth,
		// Top Face
		-HalfWidth, HalfHeight, -HalfDepth,
		HalfWidth, HalfHeight, -HalfDepth, 
		HalfWidth, HalfHeight, HalfDepth,
		-HalfWidth, HalfHeight, HalfDepth, 
		// Bottom Face
		-HalfWidth, -HalfHeight, HalfDepth,
		HalfWidth, -HalfHeight, HalfDepth, 
		HalfWidth, -HalfHeight, -HalfDepth,
		-HalfWidth, -HalfHeight, -HalfDepth,
	};
	const std::vector<float> UVCoords = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	const std::vector<float> Normals = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};
	
	const std::vector<uint32_t> Indices = {
		// Front Face
		0, 1, 2,
		0, 2, 3,
		// Right Face
		4, 5, 6,
		4, 6, 7,
		// Back Face
		8, 9, 10,
		8, 10, 11,
		// Left Face
		12, 13, 14,
		12, 14, 15,
		// Top Face
		16, 17, 18,
		16, 18, 19,
		// Bottom Face
		20, 21, 22,
		20, 22, 23
	};

	MeshData CubeMeshData(VertexPositions, Indices, Normals);
	if (MeshMaterial->HasTexture())
	{
		CubeMeshData.SetUVs(UVCoords);
	}
	return CubeMeshData;
}

bool CCube::CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos, TPointer<Entity> EntityCheck)
{
	return Utils::CheckCubeHit(RayStart, RayDirection, {m_fWidth, m_fHeight, m_fDepth}, EntityCheck, HitPos);
}