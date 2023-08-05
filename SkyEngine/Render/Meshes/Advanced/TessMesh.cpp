// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "TessMesh.h"

// Engine Includes //
#include "Camera/CameraManager.h"
#include "Render/Shaders/ShaderManager.h"
#include "Render/Shaders/UndefinedShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include "Render/Materials/Material.h"
#include "Render/Materials/MaterialType.h"

TessMesh::TessMesh(const TPointer<Entity>& InOwner,float fWidth, float fHeight, glm::vec4 _Colour)
	: CMeshComponent(InOwner)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	IndicesCount = 4;
	MeshMaterial = std::make_shared<TMaterial<CUndefinedShader>>("TessMesh", ShaderManager::GetUndefinedShader("TessProgram"));
	// MeshMaterial->Colour = _Colour;
	BindTess();
}

TessMesh::~TessMesh()
{
}

void TessMesh::Render(STransform Newtransform)
{
	// TODO: Tess mesh
	// if (MeshMaterial.Texture.IsValid())
	// {
	// 	glUseProgram(MeshMaterial.ShaderProgram);
	// 	glEnable(GL_BLEND);
	// }
	// else
	// {
	// 	glUseProgram(MeshMaterial.ShaderProgram);
	// 	glDisable(GL_BLEND);
	// }
	// glFrontFace(GL_CW);
	// FTransform ScaledUpTransform = Newtransform;
	// ScaledUpTransform.Scale *= 1.1;
	// // ABOVE CALLED FROM DERIVED RENDER
	// glUniform1i(glGetUniformLocation(MeshMaterial.ShaderProgram, "bIsTex"), MeshMaterial.Texture.IsValid());
	// glUniform1i(glGetUniformLocation(MeshMaterial.ShaderProgram, "bFog"), MeshMaterial.bFog);
	// if (MeshMaterial.Texture.IsValid())
	// {
	// 	glActiveTexture(GL_TEXTURE0);
	// 	glBindTexture(GL_TEXTURE_2D, MeshMaterial.Texture.TextureID);
	// }
	// if (MeshMaterial.bFog)
	// {
	// 	glUniform3fv(glGetUniformLocation(MeshMaterial.ShaderProgram, "cameraPos"), 1, CameraManager::GetInstance()->GetCameraPosition().ToValuePtr());
	// 	glUniform4fv(glGetUniformLocation(MeshMaterial.ShaderProgram, "vFogColor"), 1, value_ptr(Lighting::GetFogColour()));
	// 	glUniform1f(glGetUniformLocation(MeshMaterial.ShaderProgram, "StartFog"), Lighting::GetStartFogDistance());
	// 	glUniform1f(glGetUniformLocation(MeshMaterial.ShaderProgram, "EndFog"), Lighting::GetEndFogDistance());
	// }
	// if (MeshMaterial.bCullFace)
	// {
	// 	glEnable(GL_CULL_FACE);
	// }
	// else
	// {
	// 	glDisable(GL_CULL_FACE);
	// }
	//
	// //enable stencil and set stencil operation
	// if (MeshMaterial.bStencil)
	// {
	// 	glEnable(GL_STENCIL_TEST);
	// 	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	// 	//glDepthMask(GL_FALSE);
	// 	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //stPass, dpFail, bothPass
	// }
	//
	// //** 1st pass **
	// //set current stencil value
	// glStencilFunc(GL_ALWAYS, // test function
	//               1, // current value to set
	//               0xFF); //mask value,
	//
	// glStencilMask(0xFF); //enable writing to stencil buffer
	// //--> render regular sized cube // fills stencil buffer
	//
	// CameraManager::GetInstance()->SetMVP(Newtransform, MeshMaterial.ShaderProgram);
	// glBindVertexArray(vao);
	// //glDrawElements(GL_TRIANGLES, m_iIndicies, GL_UNSIGNED_INT, 0);
	// glDrawArrays(GL_PATCHES, 0, 4);
	//
	// if (MeshMaterial.bStencil)
	// {
	// 	// ** 2nd pass **
	// 	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// 	glStencilMask(0x00); //disable writing to stencil buffer
	// 	//--> render scaled up cube // write to areas where value is not equal to 1
	//
	// 	glUniform1i(glGetUniformLocation(MeshMaterial.ShaderProgram, "bIsTex"), false);
	// 	CameraManager::GetInstance()->SetMVP(ScaledUpTransform, MeshMaterial.ShaderProgram);
	// 	glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, nullptr);
	//
	// 	//disable writing to stencil mask
	// 	glStencilMask(0x00);
	// 	glDisable(GL_STENCIL_TEST);
	// }
	//
	// glBindVertexArray(0);
	// glBindTexture(GL_TEXTURE_2D, 0);
}

void TessMesh::Rebind()
{
	BindTess();
}

void TessMesh::BindTess()
{
	float fHalfWidth = m_fWidth / 2;
	float fHalfHeight = m_fHeight / 2;

	GLfloat points[] = {
		-fHalfWidth, -fHalfHeight, 0.0f,
		fHalfWidth, -fHalfHeight, 0.0f,
		fHalfWidth, fHalfHeight, 0.0f,
		-fHalfWidth, fHalfHeight, 0.0f
	};

	GLuint vbo;

	glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
	glBindVertexArray(0);
}
