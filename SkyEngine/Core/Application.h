// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// #include "CEngineWindow.h"
#include "EventListener.h"
#include "Asset/AssetManager.h"
#include "Core/Core.h"
#include "Graphics/GraphicsAPI.h"
#include "Math/Vector2.h"
#include "Render/Materials/MaterialManager.h"
#include "Render/Meshes/MeshManager.h"
#include "Render/Textures/TextureManager.h"

class Scene;
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

		virtual bool OpenScene(TPointer<Scene> NewScene);
		virtual bool ApplicationSetup(std::string ExecutablePath);
		virtual void SetupConfigs();
		virtual void SetupLogManager();
		virtual void SetupViewportLayer();

		// TODO: Add return type for error handling
		int Run(std::string ExecutablePath);
		void Quit();

		virtual void Update();
		void OnEvent(CEvent& Event) override;
		virtual void Render();
		virtual void OnExit();
		
		SVector2i MainWindowSize;

		TPointer<IGraphicsAPI> GraphicsApi;
		IPlatformInterface* PlatformInterface;
		TPointer<CRenderer> Renderer;
		EGraphicsAPI GraphicsApiType;
		TPointer<CLogManager> LogManager;
		CMeshManager MeshManager;
		CMaterialManager MaterialManager;
		CTextureManager TextureManager;
		CAssetManager AssetManager;

		// TODO: Weak pointer to not hold ref
		TPointer<CEngineWindow> GetApplicationWindow() const { return ApplicationWindow; }
		CViewportCanvas* GetViewportCanvas() const { return ViewportCanvas; }
		std::string GetProjectDirectory() const { return ProjectDirectory; }
		std::string GetContentDirectory() const { return ContentPath; }

		/* Only called internally in the engine */
		inline static Application* Get();
	protected:
		virtual void SetProjectDirectory(std::string ExecutablePath);
		
		// TODO: Remove since viewport layer should make game UI canvas
		CUICanvas* UILayer;
		CViewportCanvas* ViewportCanvas;
		
		TPointer<CEngineWindow> ApplicationWindow;
		std::string ProjectDirectory;
		std::string ContentPath;
	private:
		static Application* EngineApplication;

	};

	// To be defined in client
	Application* CreateApplication();
}

ENGINE_API SkyEngine::Application* GetApplication();
ENGINE_API TPointer<IGraphicsAPI> GetGraphicsAPI();
ENGINE_API IPlatformInterface* GetPlatformInterface();
ENGINE_API TPointer<CRenderer> GetRenderer();
ENGINE_API CMeshManager* GetMeshManager();
ENGINE_API CMaterialManager* GetMaterialManager();
ENGINE_API CTextureManager* GetTextureManager();
ENGINE_API CAssetManager* GetAssetManager();

