// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// #include "EngineWindow.h"
#include "Core/Core.h"
#include "Graphics/GraphicsAPI.h"
#include "Layers/LayerStack.h"
#include "Math/Vector2.h"

class CLayer;
class IPlatformInterface;
class EngineWindow;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

namespace SkyEngine
{	
	// TODO: Namespace? More defined name?
	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual bool ApplicationSetup();

		// TODO: Add return type for error handling
		int Run();
		void Quit();

		virtual void Update();
		virtual void OnEvent();
		virtual void RenderScene();
		virtual void ChangeSize(int w, int h);
		virtual void OnExit();

		void PushLayer(CLayer* InLayer);
		void PushOverlay(CLayer* InLayer);
		
		SVector2i MainWindowSize;

		TPointer<IGraphicsAPI> GraphicsApi;
		TPointer<IPlatformInterface> PlatformInterface;
		EGraphicsAPI GraphicsApiType;

		TPointer<EngineWindow> GetApplicationWindow() const { return ApplicationWindow; }
		
		inline static Application* Get();
	private:
		static Application* EngineApplication;

		TPointer<EngineWindow> ApplicationWindow;
		CLayerStack LayerStack;
	};

	// To be defined in client
	Application* CreateApplication();
}

ENGINE_API SkyEngine::Application* GetApplication();
ENGINE_API TPointer<IGraphicsAPI> GetGraphicsAPI();

