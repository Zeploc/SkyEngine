#include "GLFWInstance.h"

#include <format>
#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.inl>

#include "GLFWWindow.h"
#include "Camera/CameraManager.h"
#include "Entity/Entity.h"
#include "Input/CXBOXController.h"
#include "Render/Shader.h"
#include "System/LogManager.h"
#include "UI/UIElement.h"

void glfw_onError(int error, const char * description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void FocusChanged(struct GLFWwindow* window, int focused);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

GLFWInstance::GLFWInstance()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	
	// Enable debugging context
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glEnable(GL_DEBUG_OUTPUT);
	glfwSetErrorCallback(glfw_onError);
	// TODO: Debug messages
	//glDebugMessageCallback(MessageCallback, NULL);
	// glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}

void glfw_onError(int error, const char* description)
{
	// print message in Windows popup dialog box
	MessageBox(NULL, LPCWSTR(description), LPCWSTR("GLFW error"), MB_OK);
}

void GLFWInstance::WindowSetup(Pointer<IGraphicsWindow> InGraphicsWindow)
{
	Pointer<GLFWWindow> GlfwWindow = std::static_pointer_cast<GLFWWindow>(InGraphicsWindow);
	GLFWwindow* GLWindow = GlfwWindow->GetGlWindow();
	glfwMakeContextCurrent(GLWindow);

	// OpenGL init
	glewInit();

	// TODO: link properly
	glfwSetFramebufferSizeCallback(GLWindow, framebuffer_size_callback);	
	glfwSetWindowFocusCallback(GLWindow, FocusChanged);
	
	// Settings Initialised
	Shader::LoadAllDefaultShadersInCurrentContext();
		
	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CW); // Front face is Clockwise order
	glEnable(GL_CULL_FACE); // Turn on the back face culling
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLFWInstance::PreRender(Pointer<IGraphicsWindow> GraphicsWindow)
{
	Pointer<GLFWWindow> GlfwWindow = std::static_pointer_cast<GLFWWindow>(GraphicsWindow);
	GLFWwindow* GlfWwindow = GlfwWindow->GetGlWindow();
	glfwMakeContextCurrent(GlfWwindow);
	glClearColor(ClearColour.R, ClearColour.G, ClearColour.B, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const Vector2 WindowSize = GraphicsWindow->GetWindowSize();
	// TODO: Option to expoose/override viewport position
	// Fill whole window with viewport by default
	glViewport(0, 0, WindowSize.X, WindowSize.Y);
}

void GLFWInstance::StoreMVP(FTransform Transform, GLuint Program)
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

void GLFWInstance::RenderMesh(const Pointer<Mesh> Mesh, const FTransform Transform)
{
	const GLuint Program = Mesh->program;
	glUseProgram(Program);
	glFrontFace(GL_CW);	
	if (Mesh->bHasTexture)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	
	FTransform ScaledUpTransform = Transform;
	ScaledUpTransform.Scale *= 1.1f;
	// ABOVE CALLED FROM DERIVED RENDER
	glUniform1i(glGetUniformLocation(Program, "bIsTex"), Mesh->bHasTexture);
	glUniform1i(glGetUniformLocation(Program, "bFog"), Mesh->bFog);
	glUniform1i(glGetUniformLocation(Program, "bIsLit"), Mesh->bIsLit);
	// TODO: Only pass in information if bIsLit? Is info still used elsewhere/otherwise
	Lighting::PassLightingToShader(Program, Mesh->LightProperties, Transform);
	if (Mesh->bHasTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Mesh->Texture.TextureID);
	}
	if (Mesh->bReflection)
	{
		// TODO: Make cubemap and test
		if (Utils::WorldCubeMap)
		{
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(glGetUniformLocation(Program, "skybox"), 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, Utils::WorldCubeMap->EntityMesh->Texture.TextureID);
			glUniform1f(glGetUniformLocation(Program, "ReflectionSize"), 0.1f);
		}
	}
	if (Mesh->bFog)
	{
		glUniform3fv(glGetUniformLocation(Program, "cameraPos"), 1, CameraManager::GetInstance()->GetCameraPosition().ToValuePtr());
		glUniform4fv(glGetUniformLocation(Program, "vFogColor"), 1, value_ptr(Lighting::GetFogColour()));
		glUniform1f(glGetUniformLocation(Program, "StartFog"), Lighting::GetStartFogDistance());
		glUniform1f(glGetUniformLocation(Program, "EndFog"), Lighting::GetEndFogDistance());
	}
	if (Mesh->bCullFace)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
	if (Mesh->bDepthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	//enable stencil and set stencil operation
	if (Mesh->bStencil)
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

	StoreMVP(Transform, Program);
	
	glBindVertexArray(Mesh->vao);
	glDrawElements(GL_TRIANGLES, Mesh->m_iIndicies, GL_UNSIGNED_INT, nullptr);

	if (Mesh->bStencil)
	{
		// ** 2nd pass **
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); //disable writing to stencil buffer
		//--> render scaled up cube // write to areas where value is not equal to 1

		glUniform1i(glGetUniformLocation(Program, "bIsTex"), false);
		StoreMVP(ScaledUpTransform, Program);
		glDrawElements(GL_TRIANGLES, Mesh->m_iIndicies, GL_UNSIGNED_INT, nullptr);

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

void GLFWInstance::RenderUIElement(Pointer<UIElement> UserInterfaceItem)
{
	UserInterfaceItem->DrawUIElement();
}

void GLFWInstance::CleanupMesh(Pointer<Mesh> Mesh)
{
	if (!Mesh)
	{
		return;
	}
	glDeleteVertexArrays(1, &Mesh->vao);
	// TODO: Look into further cleanup
}

void GLFWInstance::PostRender(Pointer<IGraphicsWindow> GraphicsWindow)
{
	SwapBuffers(GraphicsWindow);	
}

void GLFWInstance::SwapBuffers(Pointer<IGraphicsWindow> GraphicsWindow)
{
	Pointer<GLFWWindow> GlfwWindow = std::static_pointer_cast<GLFWWindow>(GraphicsWindow);
	GLFWwindow* GlfWwindow = GlfwWindow->GetGlWindow();
	
	glfwSwapBuffers(GlfWwindow);
	glfwPollEvents();
}

void FocusChanged(GLFWwindow* window, int focused)
{
	if (focused == GLFW_TRUE)
	{
		// TODO: Focus changed link
		//OnFocusChanged.Broadcast();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string sourceStr, typeStr, severityStr;
	// Convert GLenum parameters to strings

	// printf("%s:%s[%s](%d): %s\n", sourceStr, typeStr, severityStr, id, message);


	const std::string OutputString = std::format("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	                                             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
	                                             type, severity, message);
	LogManager::GetInstance()->DisplayLogMessage(OutputString);
	fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	         ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
	         type, severity, message );
}