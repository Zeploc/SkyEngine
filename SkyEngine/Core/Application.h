// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// #include "CEngineWindow.h"
#include "EventListener.h"
#include "Core/Core.h"
#include "Graphics/GraphicsAPI.h"
#include "Math/Vector2.h"

class CViewportCanvas;
class CLogManager;
class CUICanvas;
class CCanvas;
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
		virtual void SetupLogManager();
		virtual void SetupViewportLayer();

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
		TPointer<CRenderer> Renderer;
		EGraphicsAPI GraphicsApiType;
		TPointer<CLogManager> LogManager;

		// TODO: Weak pointer to not hold ref
		TPointer<CEngineWindow> GetApplicationWindow() const { return ApplicationWindow; }
		
		inline static Application* Get();
	protected:
		// TODO: Remove since viewport layer should make game UI canvas
		CUICanvas* UILayer;
		CViewportCanvas* ViewportLayer;
		
		TPointer<CEngineWindow> ApplicationWindow;
	private:
		static Application* EngineApplication;

	};

	// To be defined in client
	Application* CreateApplication();
}

ENGINE_API SkyEngine::Application* GetApplication();
ENGINE_API TPointer<IGraphicsAPI> GetGraphicsAPI();
ENGINE_API TPointer<CRenderer> GetRenderer();

