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
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
#include "Layers/UILayer.h"
#include "Layers/ViewportLayer.h"
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

	#define CAM CameraManager::GetInstance() 

	bool Application::ApplicationSetup()
	{
		PlatformInterface = std::make_shared<WindowsPlatform>();
		LogManager::GetInstance()->Init();
		GraphicsApi = IGraphicsAPI::CreateGraphicsAPI(GraphicsApiType);

		ApplicationWindow = EngineWindow::CreateEngineWindow("Application Window", MainWindowSize, false);
		if (!ApplicationWindow)
		{
			// TODO: Error management
			return false;
		}
		ApplicationWindow->GetGraphicsWindow()->FocusWindow();

		CTimeManager::Start();
		
		PushLayer(new CViewportLayer());
		PushOverlay(new CUILayer());
		
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
		ApplicationWindow->GetGraphicsWindow()->CloseWindow();
	}

	void Application::Update()
	{
		if (bLoading)
		{
			SoundManager::GetInstance()->InitFMod();
			for (CLayer* Layer : LayerStack)
			{
				Layer->OnUpdate();
			}
		}
		else
		{
			CTimeManager::Update();
			if (CTimeManager::CanTickThisFrame())
			{
				// TODO: Once linking events system, would work backwards in layer based on highest first
				for (CLayer* Layer : LayerStack)
				{
					Layer->OnUpdate();
				}
				
				Input::GetInstance()->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
			}
		}
	}

	void Application::RenderScene()
	{
		ApplicationWindow->GetGraphicsWindow()->PreRender();		
		
		for (CLayer* Layer : LayerStack)
		{
			Layer->OnRender();
		}
		ApplicationWindow->GetGraphicsWindow()->PostRender();		
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
