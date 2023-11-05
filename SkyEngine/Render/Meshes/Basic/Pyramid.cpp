// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Pyramid.h"

// Engine Includes //
#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"
#include "Render/Materials/InternalMaterial.h"

CPyramid::~CPyramid()
{
}

MeshData CPyramid::GetMeshData()
{
	const float HalfWidth = m_fWidth / 2;
	const float HalfHeight = m_fHeight / 2;
	const float HalfDepth = m_fDepth / 2;
	
	std::vector<float> VertexPositions = {			
		-HalfWidth, -HalfHeight, -HalfDepth,
		HalfWidth, -HalfHeight, -HalfDepth,
		HalfWidth, -HalfHeight, HalfDepth, 
		-HalfWidth, -HalfHeight, HalfDepth,

		0, HalfHeight, 0 // Top Point
	};
	const std::vector<float> UVCoords = {
		0, 0,
		1, 0,
		1, 1,
		0, 1,
		
		0.5, 1, // Top Point
	};
	// Improve
	const std::vector<float> Normals = {
		-0.7f, 0.0f, -0.7f, 
		0.7f, 0.0f, -0.7f,
		0.7f, 0.0f, -0.7f,
		-0.7f, 0.0f, -0.7f,		
		0.0f, 1.0f, 0.0f,
	};

	std::vector<uint32_t> Indices = {
		0, 4, 3, // Side 1
		3, 4, 2, // Side 2
		2, 4, 1, // Side 3
		1, 4, 0, // Side 4

		3, 2, 1, // Bottom Triangle 1
		3, 1, 0 // Bottom Triangle 1
	};	
	
	MeshData PyramidMeshData(VertexPositions, Indices, Normals);
	if (MeshMaterial && MeshMaterial->HasTexture())
	{
		PyramidMeshData.SetUVs(UVCoords);
	}
	return PyramidMeshData;
	
}
