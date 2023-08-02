// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "EngineWindow.h"
#include "Core/Core.h"
#include "Graphics/GraphicsAPI.h"
#include "Math/Vector.h"
#include "Math/Vector2.h"
#include "Platform/PlatformInterface.h"

namespace SkyEngine
{	
	// TODO: Namespace? More defined name?
	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual bool ApplicationSetup();

		void Run();
		void Quit();

		virtual void Update();
		virtual void RenderScene();
		virtual void ChangeSize(int w, int h);
		virtual void OnExit();
		
		bool bLoading = true;

		SVector2 MainWindowSize;
		SVector SkyColour;

		TPointer<IGraphicsAPI> GraphicsApi;
		TPointer<IPlatformInterface> PlatformInterface;
		EGraphicsAPI GraphicsApiType;

		TPointer<EngineWindow> GetApplicationWindow() const { return ApplicationWindow; }
		
		static Application* GetApplication();
	protected:
		static Application* EngineApplication;

		TPointer<EngineWindow> ApplicationWindow;
	};

	// To be defined in client
	Application* CreateApplication();
}

ENGINE_API SkyEngine::Application* GetApplication();
ENGINE_API TPointer<IGraphicsAPI> GetGraphicsAPI();

