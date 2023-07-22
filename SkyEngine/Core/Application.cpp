// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Application.h"

// Library Includes //
#include <iostream>
#include <time.h>
#include <windows.h>
#include <Box2D/Box2D.h>
//#include <vld.h>

// OpenGL Library Includes //

// Engine Includes //
#include "Input/Input.h"
#include "Scene/SceneManager.h"
#include "Sound/SoundManager.h"
#include "System/Time.h"
#include "System/LogManager.h"
#include "Camera/CameraManager.h"
#include "Render/FrameBuffer.h"
#include "Render/Shader.h"
#include "Render/Lighting.h"
// #include "Game/Scenes/Level.h"

// make sure the winsock lib is included...
#pragma comment(lib,"ws2_32.lib")

namespace SkyEngine
{
	
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	// Types //
	using namespace std;

	#define SI Input::GetInstance()
	#define CAM CameraManager::GetInstance() 
	#define SM SceneManager::GetInstance()

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void glfw_onError(int error, const char * description);
	// void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		std::string sourceStr, typeStr, severityStr;
		// Convert GLenum parameters to strings

		// printf("%s:%s[%s](%d): %s\n", sourceStr, typeStr, severityStr, id, message);

		fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			   ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
				type, severity, message );
	}

	void Application::ApplicationSetup()
	{
		srand(unsigned int(time(NULL)));

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

		Lighting::m_v4FogColour = Vector4(SkyColour, 1.0f).ToGLM();

		GLFWmonitor* FullscreenMonitor = nullptr;
		Vector2 WindowPosition;
		bool bFullScreen = false;
		if (bFullScreen)
		{
			int* MonitorCount = new int;
			GLFWmonitor** GlfwGetMonitors = glfwGetMonitors(MonitorCount);
			FullscreenMonitor = GlfwGetMonitors[0];
		}
		else
		{
			int MonitorCount;
			GLFWmonitor** GlfwGetMonitors = glfwGetMonitors(&MonitorCount);
			int x, y, width, height;
			glfwGetMonitorWorkarea(GlfwGetMonitors[0], &x, &y, &width, &height);
			WindowPosition.X = width / 2.0f - MainWindowSize.X / 2.0f;
			WindowPosition.Y = height / 2.0f - MainWindowSize.Y / 2.0f;
		}
		window = glfwCreateWindow(MainWindowSize.X, MainWindowSize.Y, "Editor", FullscreenMonitor, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			// TODO: Error management
			return;// -1;
		}
		glfwMakeContextCurrent(window);

		CAM->Init(MainWindowSize.X, MainWindowSize.Y, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
		// CAM->SwitchProjection(EProjectionMode::Perspective);
		CAM->MainWindow = window;

		glViewport(0, 0, MainWindowSize.X, MainWindowSize.Y);
		glfwSetWindowPos(window, WindowPosition.X, WindowPosition.Y);
		
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		

		// OpenGL init
		glewInit();

		// Settings Initialised
		Init();

		// The input function registration
		SI->Init(window);
	}

	void Application::Run()
	{
		ApplicationSetup();

		while (!glfwWindowShouldClose(window))
		{
			// TODO: Create colour type
			glClearColor(SkyColour.R, SkyColour.G, SkyColour.B, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, MainWindowSize.X, MainWindowSize.Y);
			CAM->SCR_WIDTH = MainWindowSize.X;
			CAM->SCR_HEIGHT = MainWindowSize.Y;
			CAM->VIEWPORT_X = 0;
			CAM->VIEWPORT_Y = 0;

			Update();
			CAM->SCR_WIDTH = MainWindowSize.X;
			CAM->SCR_HEIGHT = MainWindowSize.Y;
			CAM->VIEWPORT_X = 0;
			CAM->VIEWPORT_Y = 0;

			RenderScene();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glfwTerminate();
		OnExit();
	}

	void glfw_onError(int error, const char* description)
	{
		// print message in Windows popup dialog box
		MessageBox(NULL, LPCWSTR(description), LPCWSTR("GLFW error"), MB_OK);
	}
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Application::Update()
	{
		if (bLoading)
		{
			SoundManager::GetInstance()->InitFMod();

			// TODO: Default scene defined elsewhere and loaded
			
			// std::shared_ptr<Level> LevelScene = std::shared_ptr<Level>(new Level("Demo Environment"));			
			// SceneManager::GetInstance()->AddScene(LevelScene);
		}
		else
		{
			double dCurrentTime = glfwGetTime();//glutGet(GLUT_ELAPSED_TIME);
			double TimeDelta = (dCurrentTime - dPrevTime);// / 1000;
			dPrevTime = glfwGetTime();//glutGet(GLUT_ELAPSED_TIME);
			CurrentTimer += TimeDelta;
			if (CurrentTimer > 1.0f / TickRate)
			{
				SceneManager::GetInstance()->UpdateCurrentScene();
				CameraManager::GetInstance()->UpdateViewMatrix();
				Time::Update();
				SI->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
				CurrentTimer = 0.0f;
			}
		}
	}

	void Application::RenderScene()
	{
		glfwMakeContextCurrent(CAM->MainWindow);
		if (bLoading)
		{
			glClearColor(0.8f, 0.8f, 0.8f, 1.0); // clear grey
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			LogManager::GetInstance()->Render();
		}
		else
		{
			SM->RenderCurrentScene();
		}
	}

	void Application::ChangeSize(int w, int h)
	{
		CAM->SCR_HEIGHT = h;
		CAM->SCR_WIDTH = w;
	}
	
	void Application::Init()
	{
		Shader::LoadAllDefaultShadersInCurrentContext();
		glCullFace(GL_BACK); // Cull the Back faces
		glFrontFace(GL_CW); // Front face is Clockwise order
		glEnable(GL_CULL_FACE); // Turn on the back face culling

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		LogManager::GetInstance()->Init();
	}

	void Application::OnExit()
	{
		Shader::CleanUp();
		SceneManager::DestoryInstance();
		CameraManager::DestoryInstance();
		//Input::DestoryInstance();
		SoundManager::DestoryInstance();
		Text::Fonts.~vector();
	}

}