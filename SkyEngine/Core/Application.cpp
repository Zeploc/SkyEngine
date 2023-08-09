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
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
#include "Layers/UILayer.h"
#include "Layers/ViewportLayer.h"
#include "Platform/Window/EngineWindow.h"
#include "Platform/Windows/WindowsPlatform.h"
#include "Render/Shaders/ShaderManager.h"
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
		MainWindowSize = SVector2i(1920, 1080);
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
		LogManager::GetInstance()->Init();
		GraphicsApi = IGraphicsAPI::CreateGraphicsAPI(GraphicsApiType);

		ApplicationWindow = CEngineWindow::CreateEngineWindow("Application Window", MainWindowSize, false);
		if (!ENSURE(ApplicationWindow != NULL, "Failed to setup application window"))
		{
			return false;
		}
		ApplicationWindow->FocusWindow();
		ApplicationWindow->SubscribeEventListener(this);

		CTimeManager::Start();
		SoundManager::GetInstance()->InitFMod();
		
		PushLayer(new CViewportLayer());
		// PushOverlay(new CUILayer());		
		
		return true;
	}

	int Application::Run()
	{
		if (ApplicationSetup())
		{
			while (!ApplicationWindow->ShouldWindowClose())
			{				
				Update();
				RenderScene();
			}
		}
		else
		{
			LogManager::GetInstance()->DisplayLogError("Application failed to setup, could not start run loop!");
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
		if (CTimeManager::CanTickThisFrame())
		{
			// TODO: Once linking events system, would work backwards in layer based on highest first
			for (CLayer* Layer : LayerStack)
			{
				Layer->OnUpdate();
			}

			// TODO:
			CMouseMovedEvent Placeholder(2, 2);
			OnEvent(Placeholder); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
		}
		ApplicationWindow->Update();
	}

	void Application::OnEvent(CEvent& Event)
	{
		for (auto it = LayerStack.end(); it != LayerStack.begin();)
		{
			const bool bHandledEvent = (*--it)->OnEvent(Event);
			if (bHandledEvent)
			{
				break;
			}
		}
	}

	void Application::RenderScene()
	{
		ApplicationWindow->PreRender();		
		
		for (CLayer* Layer : LayerStack)
		{
			Layer->OnRender();
		}
		ApplicationWindow->PostRender();
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
		
		for (CLayer* Layer : LayerStack)
		{
			Layer->OnDetach();
		}
		
		Input::DestroyInstance();
		SoundManager::DestoryInstance();
		Text::Fonts.~vector();
	}

	void Application::PushLayer(CLayer* InLayer)
	{
		LayerStack.PushLayer(InLayer);
		InLayer->OnAttach();
	}

	void Application::PushOverlay(CLayer* InLayer)
	{
		LayerStack.PushOverlay(InLayer);
		InLayer->OnAttach();
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
