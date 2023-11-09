#include "SEPCH.h"
#include "DefaultMeshes.h"

CMeshData DefaultMesh::GetCubeData()
{
	constexpr float HalfWidth = 0.5f;
	constexpr float HalfHeight = 0.5f;
	constexpr float HalfDepth = 0.5f;

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

	CMeshData CubeMeshData(VertexPositions, Indices, Normals);
	CubeMeshData.SetUVs(UVCoords);
	return CubeMeshData;
}

CMeshData DefaultMesh::GetPlaneData()
{
	const float HalfWidth = 0.5f;
	const float HalfHeight = 0.5f;
	const float HalfDepth = 0.0f;
	
	const std::vector<float> VertexPositions = {
		// Front Face
		-HalfWidth, HalfHeight, HalfDepth, 
		HalfWidth, HalfHeight, HalfDepth, 
		HalfWidth, -HalfHeight, HalfDepth,
		-HalfWidth, -HalfHeight, HalfDepth,
	};
	const std::vector<float> UVCoords = {
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
	};
	
	const std::vector<uint32_t> Indices = {
		0, 1, 2, // First Triangle
		0, 2, 3 // Second Triangle
	};

	CMeshData PlaneMeshData(VertexPositions, Indices, Normals);
	PlaneMeshData.SetUVs(UVCoords);
	return PlaneMeshData;
}

CMeshData DefaultMesh::GetSphereData()
{
	constexpr float Radius = 0.5f;
	
	constexpr uint64_t Sections = 20;
	constexpr uint64_t VertexAttrib = 12;
	constexpr uint64_t IndexPerQuad = 6;

	std::vector<float> VertexPositions(Sections * Sections * POSITION_ELEMENTS_COUNT);
	std::vector<float> UVCoords(Sections * Sections * UV_ELEMENTS_COUNT);
	std::vector<float> Normals(Sections * Sections * NORMAL_ELEMENTS_COUNT);

	double Phi = 0;

	int VertexIndex = 0;
	for (int i = 0; i < Sections; i++)
	{
		double Theta = 0;

		for (int j = 0; j < Sections; j++)
		{
			float x = static_cast<float>(cos(Phi)) * static_cast<float>(sin(Theta));
			float y = static_cast<float>(cos(Theta));
			float z = static_cast<float>(sin(Phi)) * static_cast<float>(sin(Theta));

			const int PositionStartIndex = VertexIndex * POSITION_ELEMENTS_COUNT;
			VertexPositions[PositionStartIndex] = x * Radius * 2.0f;
			VertexPositions[PositionStartIndex + 1] = y * Radius * 2.0f;
			VertexPositions[PositionStartIndex + 2] = z * Radius * 2.0f;

			const int UVStartIndex = VertexIndex * UV_ELEMENTS_COUNT;
			UVCoords[UVStartIndex] = static_cast<float>(i) / (Sections - 1);
			UVCoords[UVStartIndex + 1] = static_cast<float>(j) / (Sections - 1);
			
			const int NormalStartIndex = VertexIndex * NORMAL_ELEMENTS_COUNT;
			Normals[NormalStartIndex] = x;
			Normals[NormalStartIndex + 1] = y;
			Normals[NormalStartIndex + 2] = z;
			
			VertexIndex++;

			Theta += (3.14159265359 / (Sections - 1));
			//theta += (M_PI / (sections - 1));
		}

		//phi += (2 * M_PI) / (sections - 1);
		Phi += (2 * 3.14159265359) / (Sections - 1);
	}

	std::vector<uint32_t> Indices(Sections * Sections * IndexPerQuad);
	VertexIndex = 0;
	for (int i = 0; i < Sections; i++)
	{
		for (int j = 0; j < Sections; j++)
		{
			Indices[VertexIndex++] = (((i + 1) % Sections) * Sections) + ((j + 1) % Sections);
			Indices[VertexIndex++] = (((i + 1) % Sections) * Sections) + (j);
			Indices[VertexIndex++] = (i * Sections) + (j);

			Indices[VertexIndex++] = (i * Sections) + ((j + 1) % Sections);
			Indices[VertexIndex++] = (((i + 1) % Sections) * Sections) + ((j + 1) % Sections);
			Indices[VertexIndex++] = (i * Sections) + (j);
		}
	}

	CMeshData SphereMeshData(VertexPositions, Indices, Normals);
	SphereMeshData.SetUVs(UVCoords);
	return SphereMeshData;
}

CMeshData DefaultMesh::GetPyramidData()
{
	constexpr float HalfWidth = 0.5f;
	constexpr float HalfHeight = 0.5f;
	constexpr float HalfDepth = 0.5f;
	
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
	
	CMeshData PyramidMeshData(VertexPositions, Indices, Normals);
	PyramidMeshData.SetUVs(UVCoords);
	return PyramidMeshData;
}

CMeshData DefaultMesh::GetBoxData()
{
	constexpr float HalfWidth = 0.5f;
	constexpr float HalfHeight = 0.5f;
	constexpr float HalfDepth = 0.5f;

	// TODO: Remove redundant verticies
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

	// TODO: Fix redundant indices (lazy)
	const std::vector<uint32_t> Indices = {
		// Front Face
		0, 1, 1, 2, 2, 3, 3, 0,
		// Right Face
		4, 5, 5, 6, 6, 7, 7, 4,
		// Back Face
		8, 9, 9, 10, 10, 11, 11, 8,
		// Left Face
		12, 13, 13, 14, 14, 15, 15, 12,
		// Top Face
		16, 17, 17, 18, 18, 19, 19, 16,
		// Bottom Face
		20, 21, 21, 22, 22, 23, 23, 20
	};

	CMeshData BoxMeshData(VertexPositions, Indices, Normals);
	BoxMeshData.SetUVs(UVCoords);
	return BoxMeshData;
}
