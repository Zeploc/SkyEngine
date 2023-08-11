#include "SEPCH.h"
#include "ViewportLayer.h"

#include "Camera/CameraManager.h"
#include "Core/Application.h"
#include "Platform/Window/EngineWindow.h"
#include "Graphics/GraphicsInstance.h"
#include "Input/Input.h"
#include "Render/Lighting.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CViewportLayer::CViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow)
	: CLayer(InOwningWindow, "Viewport Layer")
{
	SkyColour = SVector(0.3f, 0.8f, 0.9f);
}

CViewportLayer::~CViewportLayer()
{
}

void CViewportLayer::OnAttach()
{
	const TPointer<CEngineWindow> EngineWindow = GetApplication()->GetApplicationWindow();
	const SVector2i MainWindowSize = EngineWindow->GetSize();
	
	Lighting::SetFogColour(SVector4(SkyColour, 1.0f));
	// TODO: Move sky colour to scene
	EngineWindow->GetGraphicsInstance()->ClearColour = SkyColour;
	
	// TODO: Change from singleton to graphics instance
	CameraManager::GetInstance()->Init(EngineWindow, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
}

void CViewportLayer::OnDetach()
{
	SceneManager::DestoryInstance();
	CameraManager::DestoryInstance();
}

void CViewportLayer::OnUpdate()
{
	SceneManager::GetInstance()->UpdateCurrentScene();
	CameraManager::GetInstance()->UpdateViewMatrix();	
}

void CViewportLayer::OnRender()
{
	// TODO: Link loading
	// if (bLoading)
	// {
	// 	// TODO: Change to graphics instance
	// 	glClearColor(0.8f, 0.8f, 0.8f, 1.0); // clear grey
	// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 	LogManager::GetInstance()->Render();
	// }
	// else
	
	// TODO: Move to scene viewport
	SceneManager::GetInstance()->RenderCurrentScene(OwningWindow.lock()->GetGraphicsInstance());
}

void CViewportLayer::OnEvent(CEvent& Event)
{
	SceneManager::GetInstance()->GetCurrentScene()->OnEvent(Event);
}
