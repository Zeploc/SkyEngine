#include "SEPCH.h"
#include "ViewportLayer.h"

#include "imgui.h"
#include "Camera/CameraManager.h"
#include "Core/Application.h"
#include "Events/ApplicationEvent.h"
#include "Platform/Window/EngineWindow.h"
#include "Graphics/GraphicsInstance.h"
#include "Input/Input.h"
#include "Render/Lighting.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CViewportLayer::CViewportLayer(TWeakPointer<CEngineWindow> InOwningWindow)
	: CCanvas(InOwningWindow, "Viewport Layer")
{
	SkyColour = SVector(0.3f, 0.8f, 0.9f);
	ViewportTexture = GetGraphicsAPI()->GetTexture("Resources/Images/StoneWall_2x2.jpg");
}

CViewportLayer::~CViewportLayer()
{
}

void CViewportLayer::OnAttach()
{
	const TPointer<CEngineWindow> EngineWindow = GetApplication()->GetApplicationWindow();
	
	Lighting::SetFogColour(SVector4(SkyColour, 1.0f));
	// TODO: Move sky colour to scene
	EngineWindow->GetGraphicsInstance()->ClearColour = SkyColour;
	
	// TODO: Change from singleton to graphics instance
	CameraManager::GetInstance()->Init(this, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
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
	// 	CLogManager::GetInstance()->Render();
	// }
	// else
	
	SceneManager::GetInstance()->RenderCurrentScene(OwningWindow.lock()->GetGraphicsInstance());
}

SVector2i CViewportLayer::GetViewportSize()
{
	const TPointer<CEngineWindow> EngineWindow = GetApplication()->GetApplicationWindow();
	const SVector2i MainWindowSize = EngineWindow->GetSize();
	return MainWindowSize;
}

SVector2i CViewportLayer::GetViewportPosition()
{
	return {0,0};
}

bool CViewportLayer::OnMouseButtonPressed(int MouseButton, int Mods)
{
	return SceneManager::GetInstance()->GetCurrentScene()->OnMouseButtonPressed(MouseButton, Mods);
}

bool CViewportLayer::OnMouseButtonReleased(int MouseButton, int Mods)
{
	return SceneManager::GetInstance()->GetCurrentScene()->OnMouseButtonReleased(MouseButton, Mods);
}

bool CViewportLayer::OnMouseMoved(SVector2i MousePos)
{
	return SceneManager::GetInstance()->GetCurrentScene()->OnMouseMoved(MousePos);
}

bool CViewportLayer::OnMouseScrolled(float XOffset, float YOffset)
{
	return SceneManager::GetInstance()->GetCurrentScene()->OnMouseScrolled(XOffset, YOffset);
}

bool CViewportLayer::OnKeyPressed(int KeyCode, int Mods, int RepeatCount)
{
	return SceneManager::GetInstance()->GetCurrentScene()->OnKeyPressed(KeyCode, Mods, RepeatCount);
}

bool CViewportLayer::OnKeyTyped(int KeyCode, int Mods)
{
	// return SceneManager::GetInstance()->GetCurrentScene()->OnKeyTyped(KeyCode, Mods);
	return false;
}

bool CViewportLayer::OnKeyReleased(int KeyCode, int Mods)
{
	return SceneManager::GetInstance()->GetCurrentScene()->OnKeyReleased(KeyCode, Mods);
}

bool CViewportLayer::OnWindowResize(unsigned int Width, unsigned int Height)
{
	// return SceneManager::GetInstance()->GetCurrentScene()->OnWindowResize(Width, Height);
	return false;
}
