// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "EngineWindow.h"
#include "Core/Core.h"
#include "Graphics/GraphicsAPI.h"
#include "Math/Vector.h"
#include "Math/Vector2.h"

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
		virtual void Init();
		virtual void OnExit();

		bool bLoading = true;

		Vector2 MainWindowSize;
		Vector3 SkyColour;

		Pointer<IGraphicsAPI> GraphicsInterface;

		Pointer<EngineWindow> GetApplicationWindow() const { return ApplicationWindow; }
		
		static Application* GetApplication();
	protected:
		static Application* EngineApplication;

		Pointer<EngineWindow> ApplicationWindow;
	};

	// To be defined in client
	Application* CreateApplication();
}

ENGINE_API SkyEngine::Application* GetApplication();

