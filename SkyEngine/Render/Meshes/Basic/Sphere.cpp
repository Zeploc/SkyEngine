// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Sphere.h"

// Engine Includes //
#include "Render/Lighting.h"
#include "Render/Shader.h"

// OpenGL Library //
#include <soil/SOIL2.h>

#include "Core/Application.h"
#include "Entity/Entity.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Sphere::Sphere(float fWidth, float fHeight, float fDepth, glm::vec4 _Colour)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	MeshMaterial.Colour = _Colour;
	BindSphere();
	m_eShape = EMESHTYPE::SPHERE;
	MeshMaterial.ShaderProgram = Shader::Programs["BaseProgram"];
	// if (bIsLit)
	// {
	// 	program = Shader::Programs["LitTextureprogram"];
	// }
	// else
	// {
	// 	program = Shader::Programs["BaseProgram"];
	// }
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Sphere::Sphere(float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, const char* _TextureSource, glm::vec4 _UVCoords)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	MeshMaterial.Colour = _Colour;
	MeshMaterial.Texture.Path = _TextureSource;
	UVCoords = _UVCoords;
	BindSphere();
	m_eShape = EMESHTYPE::SPHERE;
	MeshMaterial.ShaderProgram = Shader::Programs["BaseProgram"];
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Sphere::~Sphere()
{
}

/************************************************************
#--Description--#: 	Binds sphere with sepcified attributes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Sphere::BindSphere()
{
	constexpr uint64_t Sections = 20;
	constexpr uint64_t VertexAttrib = 12;
	constexpr uint64_t IndexPerQuad = 6;
	const Vector4 Colour = MeshMaterial.Colour;

	double phi = 0;
	double theta = 0;

	std::vector<float> vertices(Sections * Sections * VertexAttrib);
	int offset = 0;
	for (int i = 0; i < Sections; i++)
	{
		theta = 0;

		for (int j = 0; j < Sections; j++)
		{
			float x = static_cast<float>(cos(phi)) * static_cast<float>(sin(theta));
			float y = static_cast<float>(cos(theta));
			float z = static_cast<float>(sin(phi)) * static_cast<float>(sin(theta));

			vertices[offset++] = x * m_fWidth;
			vertices[offset++] = y * m_fHeight;
			vertices[offset++] = z * m_fDepth;

			vertices[offset++] = Colour.x;
			vertices[offset++] = Colour.y;
			vertices[offset++] = Colour.z;
			vertices[offset++] = Colour.a;

			vertices[offset++] = static_cast<float>(i) / (Sections - 1);
			vertices[offset++] = static_cast<float>(j) / (Sections - 1);

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			theta += (3.14159265359 / (Sections - 1));
			//theta += (M_PI / (sections - 1));
		}

		//phi += (2 * M_PI) / (sections - 1);
		phi += (2 * 3.14159265359) / (Sections - 1);
	}

	std::vector<uint32_t> indices(Sections * Sections * IndexPerQuad);
	offset = 0;
	for (int i = 0; i < Sections; i++)
	{
		for (int j = 0; j < Sections; j++)
		{
			indices[offset++] = (((i + 1) % Sections) * Sections) + ((j + 1) % Sections);
			indices[offset++] = (((i + 1) % Sections) * Sections) + (j);
			indices[offset++] = (i * Sections) + (j);

			indices[offset++] = (i * Sections) + ((j + 1) % Sections);
			indices[offset++] = (((i + 1) % Sections) * Sections) + ((j + 1) % Sections);
			indices[offset++] = (i * Sections) + (j);
		}
	}
	
	vao = GetGraphicsAPI()->CreateBuffer(MeshMaterial.Texture, true, true);
	GetGraphicsAPI()->BindArray(vertices, indices, vao, false);

	m_iIndicies = static_cast<int>(indices.size());
}

/************************************************************
#--Description--#: 	Rebinds the vao with the colour (not texture)
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Sphere::Rebind()
{
	BindSphere();
}

void Sphere::SetLit(bool _bIsLit)
{
	Mesh::SetLit(_bIsLit);
	// if (bIsLit)
	// {
	// 	program = Shader::Programs["LitTextureprogram"];
	// }
	// else
	// {
	// 	if (bHasTexture)
	// 	{
	// 		program = Shader::Programs["BaseProgram"];
	// 	}
	// 	else
	// 	{
	// 		program = Shader::Programs["BaseProgram"];
	// 	}
	// }
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Sphere::Update()
{
}

bool Sphere::CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, Pointer<Entity> EntityCheck)
{
	return Utils::CheckSphereHit(RayStart, RayDirection, EntityCheck->Transform.Position, m_fWidth, HitPos);
}
