// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "TessMesh.h"

// Engine Includes //
#include "Camera/CameraManager.h"
#include "Render/Shader.h"

#include <glm/gtc/type_ptr.hpp>

TessMesh::TessMesh(float fWidth, float fHeight, glm::vec4 _Colour)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_iIndicies = 4;
	Colour = _Colour;
	TextureSource = "";
	bHasTexture = false;
	BindTess();
	program = Shader::Programs["TessProgram"];
	UVCoords = glm::vec4(0, 1, 0, 1);
}

TessMesh::~TessMesh()
{
}

void TessMesh::Render(FTransform Newtransform)
{
	if (bHasTexture)
	{
		glUseProgram(program);
		glEnable(GL_BLEND);
	}
	else
	{
		glUseProgram(program);
		glDisable(GL_BLEND);
	}
	glFrontFace(GL_CW);
	FTransform ScaledUpTransform = Newtransform;
	ScaledUpTransform.Scale *= 1.1;
	// ABOVE CALLED FROM DERIVED RENDER
	glUniform1i(glGetUniformLocation(program, "bIsTex"), bHasTexture);
	glUniform1i(glGetUniformLocation(program, "bFog"), bFog);
	if (bHasTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	if (bFog)
	{
		glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, value_ptr(CameraManager::GetInstance()->GetCameraPosition()));
		glUniform4fv(glGetUniformLocation(program, "vFogColor"), 1, value_ptr(Lighting::GetFogColour()));
		glUniform1f(glGetUniformLocation(program, "StartFog"), Lighting::GetStartFogDistance());
		glUniform1f(glGetUniformLocation(program, "EndFog"), Lighting::GetEndFogDistance());
	}
	if (bCullFace)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	//enable stencil and set stencil operation
	if (bStencil)
	{
		glEnable(GL_STENCIL_TEST);
		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		//glDepthMask(GL_FALSE);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //stPass, dpFail, bothPass
	}

	//** 1st pass **
	//set current stencil value
	glStencilFunc(GL_ALWAYS, // test function
	              1, // current value to set
	              0xFF); //mask value,

	glStencilMask(0xFF); //enable writing to stencil buffer
	//--> render regular sized cube // fills stencil buffer

	CameraManager::GetInstance()->SetMVP(Newtransform, program);
	glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, m_iIndicies, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_PATCHES, 0, 4);

	if (bStencil)
	{
		// ** 2nd pass **
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); //disable writing to stencil buffer
		//--> render scaled up cube // write to areas where value is not equal to 1

		glUniform1i(glGetUniformLocation(program, "bIsTex"), false);
		CameraManager::GetInstance()->SetMVP(ScaledUpTransform, program);
		glDrawElements(GL_TRIANGLES, m_iIndicies, GL_UNSIGNED_INT, nullptr);

		//disable writing to stencil mask
		glStencilMask(0x00);
		glDisable(GL_STENCIL_TEST);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
