// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// #include "CEngineWindow.h"
#include "EventListener.h"
#include "Core/Core.h"
#include "Graphics/GraphicsAPI.h"
#include "Layers/LayerStack.h"
#include "Math/Vector2.h"

class CUILayer;
class CLayer;
class IPlatformInterface;
class CEngineWindow;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

namespace SkyEngine
{	
	// TODO: Namespace? More defined name?
	class ENGINE_API Application : public IEventListener//, public std::enable_shared_from_this<Application>
	{
	public:
		Application();
		virtual ~Application();

		virtual bool ApplicationSetup();

		// TODO: Add return type for error handling
		int Run();
		void Quit();

		virtual void Update();
		void OnEvent(CEvent& Event) override;
		virtual void Render();
		virtual void ChangeSize(int w, int h);
		virtual void OnExit();
		
		SVector2i MainWindowSize;

		TPointer<IGraphicsAPI> GraphicsApi;
		TPointer<IPlatformInterface> PlatformInterface;
		EGraphicsAPI GraphicsApiType;

		// TODO: Weak pointer to not hold ref
		TPointer<CEngineWindow> GetApplicationWindow() const { return ApplicationWindow; }
		
		inline static Application* Get();
	protected:
		CUILayer* UILayer;
		
	private:
		static Application* EngineApplication;

		TPointer<CEngineWindow> ApplicationWindow;
	};

	// To be defined in client
	Application* CreateApplication();
}

ENGINE_API SkyEngine::Application* GetApplication();
ENGINE_API TPointer<IGraphicsAPI> GetGraphicsAPI();

