// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GLRenderer.h"

#include <format>
#include <glm/gtc/type_ptr.inl>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "Camera/CameraManager.h"
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "Input/CXBOXController.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Shaders/Shader.h"
#include "Render/Shaders/ShaderManager.h"
#include "System/LogManager.h"
#include "UI/Legacy/UIElement.h"

GLRenderer::GLRenderer()
{	

	// TODO: Move to more relevant
	// Settings Initialised
	ShaderManager::LoadAllDefaultShadersInCurrentContext();
		
	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CW); // Front face is Clockwise order
	glEnable(GL_CULL_FACE); // Turn on the back face culling
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLRenderer::SetWireframeMode(bool bInWireframeEnabled)
{
	glPolygonMode(GL_FRONT_AND_BACK, bInWireframeEnabled ? GL_LINE : GL_FILL);
}

void GLRenderer::RenderImGui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
}

void GLRenderer::StoreMVP(STransform Transform, GLuint Program)
{
	const CameraManager* CameraInstance = CameraManager::GetInstance();
	glm::mat4 ModelMatrix = Transform.GetModelMatrix();
	glUniformMatrix4fv(glGetUniformLocation(Program, "model"), 1, GL_FALSE, value_ptr(ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(Program, "view"), 1, GL_FALSE, glm::value_ptr(CameraInstance->View.ToGLM()));//view));//
	glUniformMatrix4fv(glGetUniformLocation(Program, "proj"), 1, GL_FALSE, glm::value_ptr(CameraInstance->Projection.ToGLM()));

	glm::mat4 MVP = CameraInstance->GetMVP(ModelMatrix);
	const GLint MVPLoc = glGetUniformLocation(Program, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, value_ptr(MVP));
}

void GLRenderer::ApplyMaterialFlags(TPointer<CMaterialInterface>InMaterial)
{
	if (!InMaterial->bTwoSided)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
	if (InMaterial->bDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void GLRenderer::RenderMesh(const TPointer<CMeshComponent> Mesh, const STransform Transform)
{
	StoreMVP(Transform, ActiveShader->GetShaderProgram());
	
	glBindVertexArray(Mesh->vao);
	glDrawElements(GL_TRIANGLES, Mesh->IndicesCount, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);

	return;
	
	// TPointer<CMaterial> MeshMaterial = Mesh->MeshMaterial;
	// const CTexture TextureData = MeshMaterial->GetTextureData();
	// const GLuint Program = MeshMaterial->GetShader()->GetShaderProgram();
	// glUseProgram(Program);
	// glFrontFace(GL_CW);
	// if (TextureData.IsValid())
	// {
	// 	glEnable(GL_BLEND);
	// }
	// else
	// {
	// 	glDisable(GL_BLEND);
	// }
	//
	// // ABOVE CALLED FROM DERIVED RENDER
	// glUniform1i(glGetUniformLocation(Program, "bIsTex"), TextureData.IsValid());
	// glUniform1i(glGetUniformLocation(Program, "bFog"), MeshMaterial->bFog);
	// glUniform1i(glGetUniformLocation(Program, "bIsLit"), MeshMaterial->bIsLit);
	// // TODO: Only pass in information if bIsLit? Is info still used elsewhere/otherwise
	// Lighting::PassLightingToShader(shared_from_this());
	// if (TextureData.IsValid())
	// {
	// 	glActiveTexture(GL_TEXTURE0);
	// 	glBindTexture(GL_TEXTURE_2D, TextureData.TextureID);
	// }
	// if (MeshMaterial->bReflect)
	// {
	// 	// TODO: Make cubemap and test
	// 	// if (Utils::WorldCubeMap)
	// 	// {
	// 	// 	glActiveTexture(GL_TEXTURE1);
	// 	// 	glUniform1i(glGetUniformLocation(Program, "skybox"), 1);
	// 	// 	glBindTexture(GL_TEXTURE_CUBE_MAP, Utils::WorldCubeMap->EntityMesh->MeshMaterial.Texture.TextureID);
	// 	// 	glUniform1f(glGetUniformLocation(Program, "ReflectionSize"), 0.1f);
	// 	// }
	// }
	// if (MeshMaterial->bFog)
	// {
	// 	glUniform3fv(glGetUniformLocation(Program, "cameraPos"), 1, CameraManager::GetInstance()->GetCameraPosition().ToValuePtr());
	// 	glUniform4fv(glGetUniformLocation(Program, "vFogColor"), 1, value_ptr(Lighting::GetFogColour()));
	// 	glUniform1f(glGetUniformLocation(Program, "StartFog"), Lighting::GetStartFogDistance());
	// 	glUniform1f(glGetUniformLocation(Program, "EndFog"), Lighting::GetEndFogDistance());
	// }
	// if (!MeshMaterial->bTwoSided)
	// {
	// 	glEnable(GL_CULL_FACE);
	// }
	// else
	// {
	// 	glDisable(GL_CULL_FACE);
	// }
	// if (MeshMaterial->bDepthTest)
	// {
	// 	glEnable(GL_DEPTH_TEST);
	// }
	// else
	// {
	// 	glDisable(GL_DEPTH_TEST);
	// }
	//
	// //enable stencil and set stencil operation
	// if (MeshMaterial->bStencil)
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
	// StoreMVP(Transform, Program);
	//
	// glBindVertexArray(Mesh->vao);
	// glDrawElements(GL_TRIANGLES, Mesh->IndicesCount, GL_UNSIGNED_INT, nullptr);
	//
	// STransform ScaledUpTransform = Transform;
	// ScaledUpTransform.Scale *= 1.1f;
	// if (MeshMaterial->bStencil)
	// {
	// 	// ** 2nd pass **
	// 	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// 	glStencilMask(0x00); //disable writing to stencil buffer
	// 	//--> render scaled up cube // write to areas where value is not equal to 1
	//
	// 	glUniform1i(glGetUniformLocation(Program, "bIsTex"), false);
	// 	StoreMVP(ScaledUpTransform, Program);
	// 	glDrawElements(GL_TRIANGLES, Mesh->IndicesCount, GL_UNSIGNED_INT, nullptr);
	//
	// 	//disable writing to stencil mask
	// 	glStencilMask(0x00);
	// 	glDisable(GL_STENCIL_TEST);
	// }
	//
	// glBindVertexArray(0);
	// glBindTexture(GL_TEXTURE_2D, 0);

	// TODO: Check all errors and log any
	// CLogManager::GetInstance()->DisplayLogMessage("Open GL Error: " + std::to_string(glGetError(0)));
	// glCheckError_
}

void GLRenderer::RenderUIElement(TPointer<UIElement> UserInterfaceItem)
{
	UserInterfaceItem->DrawUIElement();
}

void GLRenderer::CleanupMesh(TPointer<CMeshComponent> Mesh)
{
	if (!Mesh)
	{
		return;
	}
	glDeleteVertexArrays(1, &Mesh->vao);
	// TODO: Look into further cleanup
}