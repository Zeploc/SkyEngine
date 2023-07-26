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
#include "Graphics/GraphicsInstance.h"
#include "Graphics/GraphicsWindow.h"
#include "Graphics/GLFW/GLFWInterface.h"
#include "Render/FrameBuffer.h"
#include "Render/Shader.h"
#include "Render/Lighting.h"
// #include "Game/Scenes/Level.h"

// make sure the winsock lib is included...
#pragma comment(lib,"ws2_32.lib")

namespace SkyEngine
{
	// TODO: Fix so not needed in derived projects
	Application* Application::EngineApplication = nullptr;
	
	Application::Application()
	{
		EngineApplication = this;
	}

	Application::~Application()
	{
	}

	// Types //
	using namespace std;

	#define SI Input::GetInstance()
	#define CAM CameraManager::GetInstance() 
	#define SM SceneManager::GetInstance()

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
		// TODO: Temp setup as GLFW
		GraphicsInterface = make_shared<GLFWInterface>();
		
		srand(unsigned int(time(NULL)));

		Lighting::SetFogColour(Vector4(SkyColour, 1.0f).ToGLM());

		ApplicationWindow = EngineWindow::CreateEngineWindow("Application Window", MainWindowSize, false);
		if (!ApplicationWindow)
		{
			// TODO: Error management
			OnExit();
		}
		ApplicationWindow->GetGraphicsWindow()->FocusWindow();
		ApplicationWindow->GetGraphicsWindow()->GetGraphicsInstance()->ClearColour = SkyColour;

		CAM->Init(MainWindowSize.X, MainWindowSize.Y, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
		// CAM->SwitchProjection(EProjectionMode::Perspective);
		CAM->MainWindow = ApplicationWindow;
		
		// Settings Initialised
		Init();

		// TODO: Move to window?
		// The input function registration
		SI->Init(ApplicationWindow);
	}

	void Application::Run()
	{
		ApplicationSetup();

		while (!ApplicationWindow->ShouldWindowClose())
		{
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
		}
		
		OnExit();
	}

	void Application::Quit()
	{
		ApplicationWindow->GetGraphicsWindow()->CloseWindow();
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
		ApplicationWindow->GetGraphicsWindow()->PreRender();		
		if (bLoading)
		{
			// TODO: Change to graphics instance
			glClearColor(0.8f, 0.8f, 0.8f, 1.0); // clear grey
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			LogManager::GetInstance()->Render();
		}
		else
		{
			SM->RenderCurrentScene();
		}
		ApplicationWindow->GetGraphicsWindow()->PostRender();		
	}

	void Application::ChangeSize(int w, int h)
	{
		CAM->SCR_HEIGHT = h;
		CAM->SCR_WIDTH = w;
	}
	
	void Application::Init()
	{
		LogManager::GetInstance()->Init();
	}

	void Application::OnExit()
	{
		ApplicationWindow.reset();
		GraphicsInterface.reset();
		Shader::CleanUp();
		SceneManager::DestoryInstance();
		CameraManager::DestoryInstance();
		//Input::DestoryInstance();
		SoundManager::DestoryInstance();
		Text::Fonts.~vector();
	}
}

SkyEngine::Application* SkyEngine::Application::GetApplication()
{
	return EngineApplication;
}

SkyEngine::Application* GetApplication()
{
	return SkyEngine::Application::GetApplication();
}
