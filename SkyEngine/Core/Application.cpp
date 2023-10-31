// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Application.h"

// Library Includes //
//#include <vld.h>

// Engine Includes //
#include "Events/MouseEvent.h"
#include "Input/Input.h"
#include "Sound/SoundManager.h"
#include "System/LogManager.h"
#include "Canvas/Canvas.h"
#include "Canvas/UICanvas.h"
#include "Canvas/ViewportCanvas.h"
#include "Platform/Window/EngineWindow.h"
#include "Platform/Windows/WindowsPlatform.h"
#include "Render/Shaders/ShaderManager.h"
#include "Scene/SceneManager.h"
#include "System/TimeManager.h"

// make sure the winsock lib is included...
#pragma comment(lib,"ws2_32.lib")

namespace SkyEngine
{
	Application* Application::EngineApplication = nullptr;

#define BIND_EVENT_FN(x) std::bind(&(x), this, std::placeholders::_1)
	
	Application::Application()
	{
		ensure(!EngineApplication, "Application already exist!");
		EngineApplication = this;
		MainWindowSize = SVector2i(1280, 720);
		GraphicsApiType = EGraphicsAPI::OPENGL;
	}

	Application::~Application()
	{
	}

	// Types //
	using namespace std;

	bool Application::ApplicationSetup()
	{
		PlatformInterface = std::make_shared<WindowsPlatform>();
		SetupLogManager();
		ENSURE(LogManager != nullptr, "Log manager not created!");
		LogManager->Init();

		ApplicationWindow = CEngineWindow::CreateEngineWindow("Sky Engine", MainWindowSize, false);
		if (!ENSURE(ApplicationWindow != NULL, "Failed to setup application window"))
		{
			return false;
		}
		ApplicationWindow->FocusWindow();
		ApplicationWindow->SubscribeEventListener(this);
		
		GraphicsApi = IGraphicsAPI::CreateGraphicsAPI(GraphicsApiType);
		Renderer = GraphicsApi->CreateNewRenderer();

		CTimeManager::Start();
		SoundManager::GetInstance()->InitFMod();

		SetupViewportLayer();
		ApplicationWindow->PushLayer(ViewportLayer);
		
		return true;
	}

	void Application::SetupLogManager()
	{
		LogManager = std::make_shared<CLogManager>();
	}
	void Application::SetupViewportLayer()
	{
		ViewportLayer = new CViewportCanvas(ApplicationWindow);
	}

	int Application::Run()
	{
		if (ApplicationSetup())
		{
			while (!ApplicationWindow->ShouldWindowClose())
			{				
				Update();
				Render();
			}
		}
		else
		{
			CLogManager::GetInstance()->DisplayLogError("Application failed to setup, could not start run loop!");
		}
		
		OnExit();
		return 0;
	}

	void Application::Quit()
	{
		ApplicationWindow->CloseWindow();
	}

	void Application::Update()
	{
		CTimeManager::Update();
		ApplicationWindow->Update();
	}

	void Application::OnEvent(CEvent& Event)
	{
	}

	void Application::Render()
	{
		ApplicationWindow->Render();
	}

	void Application::ChangeSize(int w, int h)
	{
		// TODO: Move to relevant place
	}

	void Application::OnExit()
	{
		ApplicationWindow.reset();
		GraphicsApi.reset();
		ShaderManager::CleanUp();
		// TODO: Placeholders until layer/windows properly get cleaned up
		SceneManager::DestoryInstance();
		CameraManager::DestoryInstance();
		
		SoundManager::DestoryInstance();
		Text::Fonts.~vector();
		LogManager.reset();
	}
}

SkyEngine::Application* SkyEngine::Application::Get()
{
	return EngineApplication;
}

SkyEngine::Application* GetApplication()
{
	return SkyEngine::Application::Get();
}

TPointer<IGraphicsAPI> GetGraphicsAPI()
{
	return SkyEngine::Application::Get()->GraphicsApi;
}

TPointer<CRenderer> GetRenderer()
{
	return SkyEngine::Application::Get()->Renderer;
}
