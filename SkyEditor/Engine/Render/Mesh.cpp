// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Mesh.h"

// Engine Includes //
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "Engine/Camera/CameraManager.h"
#include "Engine/Entity/CollisionBounds.h"
#include "Engine/Entity/Entity.h"
#include "Engine/System/LogManager.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Mesh::Mesh()
{

}

/************************************************************
#--Description--#:  Constructor function with texture
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Mesh::~Mesh()
{
	// TODO: Look into further cleanup
	glDeleteVertexArrays(1, &vao);
	//if (MeshCollisionBounds) delete MeshCollisionBounds;
	MeshCollisionBounds = nullptr;
	// TODO: Give id (component system?) and link to parent
	LogManager::GetInstance()->DisplayLogMessage("Mesh was destroyed!");	
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Mesh::Render(FTransform Newtransform)
{
	glUseProgram(program);
	glFrontFace(GL_CW);	
	if (bHasTexture)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	
	FTransform ScaledUpTransform = Newtransform;
	ScaledUpTransform.Scale *= 1.1f;
	// ABOVE CALLED FROM DERIVED RENDER
	glUniform1i(glGetUniformLocation(program, "bIsTex"), bHasTexture);
	glUniform1i(glGetUniformLocation(program, "bFog"), bFog);
	glUniform1i(glGetUniformLocation(program, "bIsLit"), bIsLit);
	// TODO: Only pass in information if bIsLit? Is info still used elsewhere/otherwise
	Lighting::PassLightingToShader(program, LightProperties, Newtransform);
	if (bHasTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	if (bReflection)
	{
		// TODO: Make cubemap and test
		if (Utils::WorldCubeMap)
		{
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(glGetUniformLocation(program, "skybox"), 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, Utils::WorldCubeMap->EntityMesh->texture);
			glUniform1f(glGetUniformLocation(program, "ReflectionSize"), 0.1f);
		}
	}
	if (bFog)
	{
		glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, value_ptr(CameraManager::GetInstance()->GetCameraPosition().ToGLM()));
		glUniform4fv(glGetUniformLocation(program, "vFogColor"), 1, value_ptr(Lighting::m_v4FogColour));
		glUniform1f(glGetUniformLocation(program, "StartFog"), Lighting::StartFogDistance);
		glUniform1f(glGetUniformLocation(program, "EndFog"), Lighting::EndFogDistance);
	}
	if (bCullFace)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
	if (bDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
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
	glDrawElements(GL_TRIANGLES, m_iIndicies, GL_UNSIGNED_INT, nullptr);

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

	// TODO: Check all errors and log any
	// LogManager::GetInstance()->DisplayLogMessage("Open GL Error: " + std::to_string(glGetError(0)));
	// glCheckError_
}

// GLenum glCheckError_(const char *file, int line)
// {
// 	GLenum errorCode;
// 	while ((errorCode = glGetError()) != GL_NO_ERROR)
// 	{
// 		std::string error;
// 		switch (errorCode)
// 		{
// 			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
// 			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
// 			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
// 			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
// 			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
// 			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
// 			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
// 		}
// 		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
// 	}
// 	return errorCode;
// }
// #define glCheckError() glCheckError_(__FILE__, __LINE__)

// TODO: Remove the need for entity passed through (use transforms/matrix)
bool Mesh::CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, std::shared_ptr<Entity> EntityCheck)
{
	// TODO: Check against basic box bounds before considering
	
	LogManager::GetInstance()->DisplayLogMessage("No Check Hit for mesh ray hit check!");

	return false;
}

void Mesh::Reset()
{
	// Reset all mesh variables
	Rebind();
	// Reset Mesh Collision Bounds
	if (MeshCollisionBounds)
	{
		MeshCollisionBounds->Reset();
	}
}

void Mesh::SetLit(bool _bIsLit)
{
	bIsLit = _bIsLit;
}

void Mesh::SetReflection(bool _bReflecting)
{
	bReflection = _bReflecting;
	// if (bReflection)
	// {
	// 	program = Shader::Programs["ReflectionProgram"];
	// }
	// else
	// {
	// 	if (bIsLit)
	// 	{
	// 		program = Shader::Programs["LitTextureprogram"];
	// 	}
	// 	else
	// 	{
	// 		program = Shader::Programs["BaseProgram"];
	// 	}
	// }
}

void Mesh::AddCollisionBounds(float fWidth, float fHeight, float fDepth, std::shared_ptr<Entity> _EntityRef)
{
	MeshCollisionBounds = std::make_shared<CollisionBounds>(fWidth, fHeight, fDepth, _EntityRef);
}

void Mesh::AddCollisionBounds(std::shared_ptr<CollisionBounds> NewCollision)
{
	MeshCollisionBounds = NewCollision;
}
