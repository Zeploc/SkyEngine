#include "SEPCH.h"
#include "ViewportCanvas.h"

#include "imgui.h"
#include "Core/Application.h"
#include "Core/WorldManager.h"
#include "Entity/Camera.h"
#include "Render/Renderer.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Lighting.h"
#include "Render/SceneRenderer.h"
#include "Scene/SceneManager.h"

CViewportCanvas::CViewportCanvas(TWeakPointer<CEngineWindow> InOwningWindow)
	: CCanvas(InOwningWindow, "Viewport Layer")
{
	SkyColour = SVector(0.3f, 0.8f, 0.9f);
	WorldManager = CreatePointer<CWorldManager>();
	// TODO: Would instead be context based ie what is the viewport opening
	WorldManager->SwitchScene(SceneManager::GetInstance()->GetCurrentScene());
}

CViewportCanvas::~CViewportCanvas()
{
}

void CViewportCanvas::OnAttach()
{
	const THardPointer<CEngineWindow> EngineWindow = GetApplication()->GetApplicationWindow();
	ViewportSize = EngineWindow->GetSize();
	
	Lighting::SetFogColour(SVector4(SkyColour, 1.0f));
	const TAssetObjectPointer<Scene> TargetScene = SceneManager::GetInstance()->GetCurrentScene();

	SceneRenderer = GetRenderer()->AddSceneRenderer(TargetScene, ViewportSize);
	SceneRenderer->ClearColour = SkyColour;
}

void CViewportCanvas::OnDetach()
{
}

void CViewportCanvas::OnUpdate()
{
}

void CViewportCanvas::OnRender()
{
}

void CViewportCanvas::OnViewportResize()
{
	SceneRenderer->SizeChanged(ViewportSize);
}

void CViewportCanvas::SetupCamera()
{	
	SetCamera(CreatePointer<Camera>(STransform(SVector(-10.0f, 10.0f, 10.0f), SRotator(-90.0f, 0.0f, 0.0f))));
}

void CViewportCanvas::SetCamera(THardPointer<Camera> NewCamera)
{
	ViewportCamera = NewCamera;
	SceneRenderer->LinkCamera(ViewportCamera);
}

SVector2i CViewportCanvas::GetViewportSize()
{
	return ViewportSize;
}

SVector2i CViewportCanvas::GetViewportPosition()
{
	return {0,0};
}

SVector CViewportCanvas::ScreenToWorldDirection(SVector2i InScreenPosition)
{
	SVector2i ViewportPosition = GetViewportPosition();
	SVector2i ViewportSize = GetViewportSize();
	float x = (2.0f * ((float)InScreenPosition.X - ViewportPosition.X)) / (float)ViewportSize.X - 1.0f;
	float y = 1.0f - (2.0f * ((float)InScreenPosition.Y - ViewportPosition.Y)) / (float)ViewportSize.Y;
	const SVector2 RayNds = {x, y};

	const SVector4 RayClip = SVector4(RayNds.X, RayNds.Y, -1.0f, 1.0f);
	
	SVector4 RayEye = SceneRenderer->GetProjection().GetInverse() * RayClip;
	RayEye = SVector4(RayEye.X, RayEye.Y, -1.0f, 0.0f);

	SVector RayWorld = SVector(SceneRenderer->GetView().GetInverse().ToGLM() * RayEye);

	RayWorld = RayWorld.Normalize();
	return RayWorld;
}

SVector CViewportCanvas::ScreenToWorldPosition2D(SVector2i InScreenPosition)
{
	const SVector CameraPosition = ViewportCamera->Transform.Position;
	const SVector PlaneNormal = -ViewportCamera->GetForwardVector();
	const SVector MouseDirection = ScreenToWorldDirection(InScreenPosition);
	float t = -(CameraPosition.Dot(PlaneNormal) / MouseDirection.Dot(PlaneNormal));
	t /= abs(CameraPosition.Z);
	return (MouseDirection * t) + CameraPosition;
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
