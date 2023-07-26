// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include "EngineWindow.h"
#include "Core/Core.h"
#include "Graphics/GraphicsInterface.h"
#include "Math/Vector.h"

namespace SkyEngine
{
	// TODO: Namespace? More defined name?
	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void ApplicationSetup();

		void Run();
		void Quit();

		virtual void Update();
		virtual void RenderScene();
		virtual void ChangeSize(int w, int h);
		virtual void Init();
		virtual void OnExit();

		bool bLoading = true;

		float CurrentTimer = 0.0f;
		float TickRate = 60.0f;
		double dPrevTime = 0.0f;

		Vector2 MainWindowSize = Vector2(1920, 1080);
		Vector3 SkyColour = Vector3(0.3f, 0.8f, 0.9f);

		std::shared_ptr<IGraphicsInterface> GraphicsInterface;

		std::shared_ptr<EngineWindow> GetApplicationWindow() const { return ApplicationWindow; }
		
		static Application* GetApplication();
	protected:
		static Application* EngineApplication;

		std::shared_ptr<EngineWindow> ApplicationWindow;
	};

	// To be defined in client
	Application* CreateApplication();
}

ENGINE_API SkyEngine::Application* GetApplication();

