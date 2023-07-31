// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "GeometryObject.h"

// OpenGL Library //

// Engine Includes //
#include "Render/Shader.h"
#include "Camera/CameraManager.h"
#include "Core/Application.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
GeometryObject::GeometryObject(glm::vec4 _Colour)
{
	m_fWidth = 1;
	m_fHeight = 1;
	m_iIndicies = 1;
	MeshMaterial.Colour = _Colour;
	BindGeometryObject();
	m_eShape = EMESHTYPE::GEOMETRY;
	MeshMaterial.ShaderProgram = Shader::Programs["GeometryShader"];
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
	GLfloat vertices[] = {
		// Position						// Color		
		0.0f, 0.0f, 0.0f, MeshMaterial.Colour.r, MeshMaterial.Colour.g, MeshMaterial.Colour.b, MeshMaterial.Colour.a, //passing in 1 point
	};

	// If no texture, texture source is equal to ""
	vao = GetGraphicsAPI()->CreateBuffer(MeshMaterial.Texture, true);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

/************************************************************
#--Description--#: 	Rebinds the vao with the colour (not texture)
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void GeometryObject::Rebind()
{
	BindGeometryObject();
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void GeometryObject::Render(FTransform Newtransform)
{
	glUseProgram(MeshMaterial.ShaderProgram);
	if (MeshMaterial.Texture.IsValid())
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	glFrontFace(GL_CW);
	glDisable(GL_CULL_FACE);
	//Mesh::Render(Newtransform);

	CameraManager::GetInstance()->SetMVP(Newtransform, MeshMaterial.ShaderProgram);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
