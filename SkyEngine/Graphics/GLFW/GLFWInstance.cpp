#include "GLFWInstance.h"

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include "GLFWWindow.h"
#include "Entity/Entity.h"
#include "Input/CXBOXController.h"
#include "Render/Shader.h"
#include "UI/UIElement.h"

void glfw_onError(int error, const char * description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void FocusChanged(struct GLFWwindow* window, int focused);

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
	// glDebugMessageCallback(MessageCallback, NULL);
	// glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}

void glfw_onError(int error, const char* description)
{
	// print message in Windows popup dialog box
	MessageBox(NULL, LPCWSTR(description), LPCWSTR("GLFW error"), MB_OK);
}

void GLFWInstance::SwapBuffers(Pointer<IGraphicsWindow> GraphicsWindow)
{
	Pointer<GLFWWindow> GlfwWindow = std::static_pointer_cast<GLFWWindow>(GraphicsWindow);
	GLFWwindow* GlfWwindow = GlfwWindow->GetGlWindow();
	
	glfwSwapBuffers(GlfWwindow);
	glfwPollEvents();
}

void GLFWInstance::Render(Pointer<IGraphicsWindow> GraphicsWindow, std::vector<Pointer<Entity>> Entities, std::vector<Pointer<UIElement>> UIElements)
{
	Pointer<GLFWWindow> GlfwWindow = std::static_pointer_cast<GLFWWindow>(GraphicsWindow);
	GLFWwindow* GlfWwindow = GlfwWindow->GetGlWindow();
	glfwMakeContextCurrent(GlfWwindow);
	glEnable(GL_DEPTH_TEST);
	for (unsigned int i = 0; i < Entities.size(); i++)
	{
		Entities[i]->DrawEntity();
	}
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	for (auto UIElement : UIElements)
	{
		UIElement->DrawUIElement();
	}
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

void GLFWInstance::PostRender(Pointer<IGraphicsWindow> GraphicsWindow)
{
	SwapBuffers(GraphicsWindow);	
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
