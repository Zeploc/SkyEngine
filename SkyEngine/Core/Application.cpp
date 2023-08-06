// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Application.h"

// Library Includes //
//#include <vld.h>

// Engine Includes //
#include "EngineWindow.h"
#include "Input/Input.h"
#include "Scene/SceneManager.h"
#include "Sound/SoundManager.h"
#include "System/LogManager.h"
#include "Camera/CameraManager.h"
#include "Graphics/GraphicsInstance.h"
#include "Math/Vector4.h"
#include "Platform/Window/GraphicsWindow.h"
#include "Platform/Windows/WindowsPlatform.h"
#include "Render/Shaders/ShaderManager.h"
#include "Render/Lighting.h"
#include "System/TimeManager.h"

// make sure the winsock lib is included...
#pragma comment(lib,"ws2_32.lib")

namespace SkyEngine
{
	Application* Application::EngineApplication = nullptr;
	
	Application::Application()
	{
		EngineApplication = this;
		MainWindowSize = SVector2i(1920, 1080);
		SkyColour = SVector(0.3f, 0.8f, 0.9f);
		GraphicsApiType = EGraphicsAPI::OPENGL;
	}

	Application::~Application()
	{
	}

	// Types //
	using namespace std;

	#define SI Input::GetInstance()
	#define CAM CameraManager::GetInstance() 
	#define SM SceneManager::GetInstance()

	bool Application::ApplicationSetup()
	{
		PlatformInterface = std::make_shared<WindowsPlatform>();
		LogManager::GetInstance()->Init();
		GraphicsApi = IGraphicsAPI::CreateGraphicsAPI(GraphicsApiType);

		Lighting::SetFogColour(SVector4(SkyColour, 1.0f));

		ApplicationWindow = EngineWindow::CreateEngineWindow("Application Window", MainWindowSize, false);
		if (!ApplicationWindow)
		{
			// TODO: Error management
			return false;
		}
		ApplicationWindow->GetGraphicsWindow()->FocusWindow();
		// TODO: Move sky colour to scene
		ApplicationWindow->GetGraphicsWindow()->GetGraphicsInstance()->ClearColour = SkyColour;

		TimeManager::Start();

		// TODO: Change from singleton to graphics instance
		CAM->Init(MainWindowSize.X, MainWindowSize.Y, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
		CAM->MainWindow = ApplicationWindow;
		
		return true;
	}

	int Application::Run()
	{
		if (ApplicationSetup())
		{
			while (!ApplicationWindow->ShouldWindowClose())
			{
				// TODO: odd... find reason and remove
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
		}
		else
		{
			// TODO: Log error
		}
		
		OnExit();
		return 0;
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
			
			// Pointer<Level> LevelScene = Pointer<Level>(new Level("Demo Environment"));			
			// SceneManager::GetInstance()->AddScene(LevelScene);
		}
		else
		{
			TimeManager::Update();
			if (TimeManager::CanTickThisFrame())
			{
				SceneManager::GetInstance()->UpdateCurrentScene();
				CameraManager::GetInstance()->UpdateViewMatrix();
				SI->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
			}
		}
	}

	void Application::RenderScene()
	{
		ApplicationWindow->GetGraphicsWindow()->PreRender();
		// TODO: Link loading
		// if (bLoading)
		// {
		// 	// TODO: Change to graphics instance
		// 	glClearColor(0.8f, 0.8f, 0.8f, 1.0); // clear grey
		// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 	LogManager::GetInstance()->Render();
		// }
		// else
		{
			SM->RenderCurrentScene();
		}
		ApplicationWindow->GetGraphicsWindow()->PostRender();		
	}

	void Application::ChangeSize(int w, int h)
	{
		// TODO: Move to relevant place
		CAM->SCR_HEIGHT = h;
		CAM->SCR_WIDTH = w;
	}

	void Application::OnExit()
	{
		ApplicationWindow.reset();
		GraphicsApi.reset();
		ShaderManager::CleanUp();
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

TPointer<IGraphicsAPI> GetGraphicsAPI()
{
	return SkyEngine::Application::GetApplication()->GraphicsApi;
}
