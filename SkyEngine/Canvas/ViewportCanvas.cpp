#include "SEPCH.h"
#include "ViewportCanvas.h"

#include "imgui.h"
#include "Camera/CameraManager.h"
#include "Core/Application.h"
#include "Core/WorldManager.h"
#include "Render/Renderer.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Lighting.h"
#include "Render/SceneRenderer.h"
#include "Scene/SceneManager.h"

CViewportCanvas::CViewportCanvas(TWeakPointer<CEngineWindow> InOwningWindow)
	: CCanvas(InOwningWindow, "Viewport Layer")
{
	SkyColour = SVector(0.3f, 0.8f, 0.9f);
	ViewportTexture = GetGraphicsAPI()->GetTexture("Resources/Images/StoneWall_2x2.jpg");
	WorldManager = CreatePointer<CWorldManager>();
	// TODO: Would instead be context based ie what is the viewport opening
	WorldManager->SwitchScene(SceneManager::GetInstance()->GetCurrentScene());
}

CViewportCanvas::~CViewportCanvas()
{
}

void CViewportCanvas::OnAttach()
{
	const TPointer<CEngineWindow> EngineWindow = GetApplication()->GetApplicationWindow();
	ViewportSize = EngineWindow->GetSize();
	
	Lighting::SetFogColour(SVector4(SkyColour, 1.0f));

	SceneRenderer = GetRenderer()->AddSceneRenderer(SceneManager::GetInstance()->GetCurrentScene(), ViewportSize);
	SceneRenderer->ClearColour = SkyColour;
	
	// TODO: Change from singleton to graphics instance
	CameraManager::GetInstance()->Init(this, glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1.0f, 0.0f));
}

void CViewportCanvas::OnDetach()
{
}

void CViewportCanvas::OnUpdate()
{
	SceneManager::GetInstance()->UpdateCurrentScene();
	CameraManager::GetInstance()->UpdateViewMatrix();
}

void CViewportCanvas::OnRender()
{
}

void CViewportCanvas::OnViewportResize()
{
	SceneRenderer->SizeChanged(ViewportSize);
}

SVector2i CViewportCanvas::GetViewportSize()
{
	return ViewportSize;
}

SVector2i CViewportCanvas::GetViewportPosition()
{
	return {0,0};
}

bool CViewportCanvas::OnMouseButtonPressed(int MouseButton, int Mods)
{
	return WorldManager->OnMouseButtonPressed(MouseButton, Mods);
}

bool CViewportCanvas::OnMouseButtonReleased(int MouseButton, int Mods)
{
	return WorldManager->OnMouseButtonReleased(MouseButton, Mods);
}

bool CViewportCanvas::OnMouseMoved(SVector2i MousePos)
{
	return WorldManager->OnMouseMoved(MousePos);
}

bool CViewportCanvas::OnMouseScrolled(float XOffset, float YOffset)
{
	return WorldManager->OnMouseScrolled(XOffset, YOffset);
}

bool CViewportCanvas::OnKeyPressed(int KeyCode, int Mods, int RepeatCount)
{
	return WorldManager->OnKeyPressed(KeyCode, Mods, RepeatCount);
}

bool CViewportCanvas::OnKeyTyped(int KeyCode, int Mods)
{
	// return WorldManager->OnKeyTyped(KeyCode, Mods);
	return false;
}

bool CViewportCanvas::OnKeyReleased(int KeyCode, int Mods)
{
	return WorldManager->OnKeyReleased(KeyCode, Mods);
}

bool CViewportCanvas::OnWindowResize(unsigned int Width, unsigned int Height)
{
	ViewportSize = SVector2i(Width, Height);
	OnViewportResize();
	// return SceneManager::GetInstance()->GetCurrentScene()->OnWindowResize(Width, Height);
	return false;
}
