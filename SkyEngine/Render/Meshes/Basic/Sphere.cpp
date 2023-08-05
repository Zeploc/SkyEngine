// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Sphere.h"

// Engine Includes //
#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "Render/Materials/InternalMaterial.h"

CSphere::~CSphere()
{
}

bool CSphere::CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos, TPointer<Entity> EntityCheck)
{
	return Utils::CheckSphereHit(RayStart, RayDirection, EntityCheck->Transform.Position, m_fWidth, HitPos);
}

MeshData CSphere::GetMeshData()
{
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
			VertexPositions[PositionStartIndex] = x * m_fWidth;
			VertexPositions[PositionStartIndex + 1] = y * m_fHeight;
			VertexPositions[PositionStartIndex + 2] = z * m_fDepth;

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

	MeshData SphereMeshData(VertexPositions, Indices, Normals);
	if (MeshMaterial->HasTexture())
	{
		SphereMeshData.SetUVs(UVCoords);
	}
	return SphereMeshData;
	
}
