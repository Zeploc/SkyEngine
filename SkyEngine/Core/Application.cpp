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
#include "Render/Renderer.h"
#include "Render/SceneRenderer.h"
#include "Render/Meshes/MeshManager.h"
#include "Scene/SceneUtils.h"
#include "Entity/Camera.h"
#include "Platform/Config/Config.h"
#include "Platform/Config/ProjectSettingsConfig.h"

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

	bool Application::OpenScene(TPointer<Scene> NewScene)
	{
		TPointer<Scene> PreviousScene = SceneManager::GetInstance()->GetCurrentScene();
		SceneManager::GetInstance()->AddScene(NewScene);
		SceneManager::GetInstance()->SwitchScene(NewScene->SceneName, true);
		const TPointer<Camera> FoundCamera = SceneUtils::FindEntityOfClass<Camera>();
		ViewportCanvas->GetSceneRenderer()->SetSceneTarget(NewScene);
		if (FoundCamera)
		{
			ViewportCanvas->SetCamera(FoundCamera);
		}
		else
		{
			ViewportCanvas->SetupCamera();
			NewScene->AddEntity(ViewportCanvas->GetViewportCamera());
		}
		SceneManager::GetInstance()->RemoveScene(PreviousScene);
		return true;
	}

	// Types //
	using namespace std;

	bool Application::ApplicationSetup(std::string ExecutablePath)
	{
		PlatformInterface = new WindowsPlatform();
		SetProjectDirectory(ExecutablePath);
		SetupLogManager();
		ENSURE(LogManager != nullptr, "Log manager not created!");
		LogManager->Init();

		SetupConfigs();

		const std::string WindowName = CProjectSettingsConfig::Get()->ProjectName;
		ApplicationWindow = CEngineWindow::CreateEngineWindow(WindowName, MainWindowSize, false);
		if (!ENSURE(ApplicationWindow != NULL, "Failed to setup application window"))
		{
			return false;
		}
		ApplicationWindow->FocusWindow();
		ApplicationWindow->SubscribeEventListener(this);
		
		GraphicsApi = IGraphicsAPI::CreateGraphicsAPI(GraphicsApiType);
		ApplicationWindow->SetupWindow();
		ShaderManager::LoadAllDefaultShaders();
		Renderer = CreatePointer<CRenderer>();

		CTimeManager::Start();
		MeshManager.Init();
		SoundManager::GetInstance()->InitFMod();

		SetupViewportLayer();
		ApplicationWindow->PushLayer(ViewportCanvas);
		
		return true;
	}
	
	void Application::SetProjectDirectory(const std::string ExecutablePath)
	{	
		uint64_t DirectoryEndIndex = ExecutablePath.find_last_of("\\");
		ProjectDirectory = ExecutablePath.substr(0, DirectoryEndIndex);
		DirectoryEndIndex = ProjectDirectory.find_last_of("\\");
		ProjectDirectory = ProjectDirectory.substr(0, DirectoryEndIndex);
		DirectoryEndIndex = ProjectDirectory.find_last_of("\\");
		ProjectDirectory = ProjectDirectory.substr(0, DirectoryEndIndex);
		DirectoryEndIndex = ProjectDirectory.find_last_of("\\");
		ProjectDirectory = ProjectDirectory.substr(0, DirectoryEndIndex);
		
		// TODO: Should be moved out of editor
		ContentPath = ProjectDirectory + "\\SkyEditor\\Assets\\";
	}

	void Application::SetupConfigs()
	{
		CConfig::RegisterConfig<CProjectSettingsConfig>();
		LogManager->DisplayMessage("Project Name: " + CProjectSettingsConfig::Get()->ProjectName); 

		MainWindowSize = TVector2<int>(CProjectSettingsConfig::Get()->DefaultResolution);
		GraphicsApiType = (EGraphicsAPI)CProjectSettingsConfig::Get()->GraphicsMode;
	}

	void Application::SetupLogManager()
	{
		LogManager = std::make_shared<CLogManager>();
	}
	void Application::SetupViewportLayer()
	{
		ViewportCanvas = new CViewportCanvas(ApplicationWindow);
	}

	int Application::Run(std::string ExecutablePath)
	{
		if (ApplicationSetup(ExecutablePath))
		{
			while (!ApplicationWindow->ShouldWindowClose())
			{				
				Update();
				Render();
			}
		}
		else
		{
			CLogManager::Get()->DisplayError("Application failed to setup, could not start run loop!");
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
		SceneManager::GetInstance()->UpdateCurrentScene();
		ApplicationWindow->Update();
	}

	void Application::OnEvent(CEvent& Event)
	{
	}

	void Application::Render()
	{
		ApplicationWindow->Render();
	}

	void Application::OnExit()
	{
		ApplicationWindow.reset();
		GraphicsApi.reset();
		ShaderManager::CleanUp();
		// TODO: Placeholders until layer/windows properly get cleaned up
		SceneManager::DestoryInstance();
		
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

IPlatformInterface* GetPlatformInterface()
{
	return SkyEngine::Application::Get()->PlatformInterface;
}

TPointer<CRenderer> GetRenderer()
{
	return SkyEngine::Application::Get()->Renderer;
}

CMeshManager* GetMeshManager()
{
	return &SkyEngine::Application::Get()->MeshManager;
}

CMaterialManager* GetMaterialManager()
{
	return &SkyEngine::Application::Get()->MaterialManager;
}

CTextureManager* GetTextureManager()
{
	return &SkyEngine::Application::Get()->TextureManager;
}

CAssetManager* GetAssetManager()
{
	return &SkyEngine::Application::Get()->AssetManager;
}
