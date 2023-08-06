// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GLInstance.h"

#include <format>
#include "GLIncludes.h"
#include <glm/gtc/type_ptr.inl>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "Camera/CameraManager.h"
#include "Entity/Entity.h"
#include "Input/CXBOXController.h"
#include "Platform/Window/GraphicsWindow.h"
#include "Render/Materials/InternalMaterial.h"
#include "Render/Shaders/Shader.h"
#include "Render/Shaders/ShaderManager.h"
#include "System/LogManager.h"
#include "UI/Legacy/UIElement.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

GLInstance::GLInstance()
{	
	glEnable(GL_DEBUG_OUTPUT);
	// TODO: Debug messages
	//glDebugMessageCallback(MessageCallback, NULL);
	// glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	
	// OpenGL init
	glewInit();

	// TODO: Move to more relevant
	// Settings Initialised
	ShaderManager::LoadAllDefaultShadersInCurrentContext();
		
	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CW); // Front face is Clockwise order
	glEnable(GL_CULL_FACE); // Turn on the back face culling
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLInstance::PreRender(TPointer<IGraphicsWindow> GraphicsWindow)
{
	glClearColor(ClearColour.R, ClearColour.G, ClearColour.B, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const SVector2i WindowSize = GraphicsWindow->GetWindowSize();
	// TODO: Option to expoose/override viewport position
	// Fill whole window with viewport by default
	glViewport(0, 0, WindowSize.X, WindowSize.Y);
}

void GLInstance::PassAttributeToShader(int32_t ShaderID, float Attribute)
{
	glUniform1f(ShaderID, Attribute);
}

void GLInstance::PassAttributeToShader(int32_t ShaderID, int Attribute)
{
	glUniform1i(ShaderID, Attribute);
}

void GLInstance::PassAttributeToShader(int32_t ShaderID, bool Attribute)
{
	glUniform1i(ShaderID, Attribute);
}

void GLInstance::PassAttributeToShader(int32_t ShaderID, SVector Attribute)
{
	glUniform3fv(ShaderID, 1, Attribute.ToValuePtr());
}

void GLInstance::PassAttributeToShader(int32_t ShaderID, SVector4 Attribute)
{
	glUniform4fv(ShaderID, 1, Attribute.ToValuePtr());
}

void GLInstance::PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute)
{
	glUniformMatrix4fv(ShaderLocation, 1, GL_FALSE, value_ptr(Attribute.ToGLM()));
}

void GLInstance::PassAttributeToShader(int32_t ShaderLocation, TPointer<CTexture> Attribute)
{
	if (!Attribute || !Attribute->IsValid())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}
	// Use shader location for index?
	glEnable(GL_BLEND);
	// TODO: Handle multiple
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Attribute->TextureID);	
}

void GLInstance::SetWireframeMode(bool bInWireframeEnabled)
{
	glPolygonMode(GL_FRONT_AND_BACK, bInWireframeEnabled ? GL_LINE : GL_FILL);
}

void GLInstance::SetRenderViewport(const SVector2i InViewportPosition, const SVector2i InViewportSize)
{
	// TODO: Safety checks
	glViewport(InViewportPosition.X, InViewportPosition.Y, InViewportSize.X, InViewportSize.Y);
}

void GLInstance::RenderImGui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
}

void GLInstance::BindShader(uint32_t ShaderProgramID)
{
	// TODO: Check if overhead and not change if current shader program is active
	glUseProgram(ShaderProgramID);	
	glFrontFace(GL_CW);
	// TODO: properly work out where blending should be changed
	// In case it was enabled
	glDisable(GL_BLEND);
	// Clear previous texture?
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLInstance::StoreMVP(STransform Transform, GLuint Program)
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

void GLInstance::ApplyMaterialFlags(TPointer<CMaterialInterface>InMaterial)
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

void GLInstance::RenderMesh(const TPointer<CMeshComponent> Mesh, const STransform Transform)
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

void GLInstance::RenderUIElement(TPointer<UIElement> UserInterfaceItem)
{
	UserInterfaceItem->DrawUIElement();
}

void GLInstance::CleanupMesh(TPointer<CMeshComponent> Mesh)
{
	if (!Mesh)
	{
		return;
	}
	glDeleteVertexArrays(1, &Mesh->vao);
	// TODO: Look into further cleanup
}

void GLInstance::PostRender(TPointer<IGraphicsWindow> GraphicsWindow)
{	
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string sourceStr, typeStr, severityStr;
	// Convert GLenum parameters to strings

	// printf("%s:%s[%s](%d): %s\n", sourceStr, typeStr, severityStr, id, message);


	const std::string OutputString = std::format("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	                                             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
	                                             type, severity, message);
	LogManager::GetInstance()->DisplayLogError(OutputString);
	// fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	//          ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
	//          type, severity, message );
}