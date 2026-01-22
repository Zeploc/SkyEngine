// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GeometryObject.h"

// OpenGL Library //

// Engine Includes //
#include "Render/Shaders/ShaderManager.h"
#include "Core/Application.h"
#include "Render/Materials/Material.h"
#include "Render/Materials/MaterialType.h"
#include "Render/Shaders/UndefinedShader.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
GeometryObject::GeometryObject(const TSharedPointer<Entity>& InOwner, glm::vec4 _Colour)
	: CMeshComponent(InOwner)
{
	LEGACY_Width = 1;
	LEGACY_Height = 1;
	// IndicesCount = 1;
	
	MeshMaterial = std::make_shared<TMaterial<CUndefinedShader>>("GeometryObjectMaterial", ShaderManager::GetUndefinedShader("GeometryShader"));
	// MeshMaterial->Colour = _Colour;
	BindGeometryObject();
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
GeometryObject::~GeometryObject()
{
}

/************************************************************
#--Description--#: 	Binds plane with sepcified attributes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void GeometryObject::BindGeometryObject()
{
	// TODO: Geo object
	// GLfloat vertices[] = {
	// 	// Position						// Color		
	// 	0.0f, 0.0f, 0.0f, MeshMaterial.Colour.r, MeshMaterial.Colour.g, MeshMaterial.Colour.b, MeshMaterial.Colour.a, //passing in 1 point
	// };
	//
	// // If no texture, texture source is equal to ""
	// vao = GetGraphicsAPI()->CreateVertexBuffer(MeshMaterial.Texture, true);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void GeometryObject::Render(STransform Newtransform)
{
	// TODO: Geo object
	// glUseProgram(MeshMaterial.ShaderProgram);
	// if (MeshMaterial.Texture.IsValid())
	// {
	// 	glEnable(GL_BLEND);
	// }
	// else
	// {
	// 	glDisable(GL_BLEND);
	// }
	// glFrontFace(GL_CW);
	// glDisable(GL_CULL_FACE);
	// //Mesh::Render(Newtransform);
	//
	// CameraManager::GetInstance()->SetMVP(Newtransform, MeshMaterial.ShaderProgram);
	//
	// glBindVertexArray(vao);
	// glDrawArrays(GL_POINTS, 0, 1);
	// glBindVertexArray(0);
}

// CMesh GeometryObject::GetMeshData() const
// {
// 	const std::vector<float> VertexPositions;
// 	const std::vector<float> Normals;
// 	const std::vector<uint32_t> Indices;
// 	// TODO:
// 	return CMesh(VertexPositions, Indices, Normals);
// }
