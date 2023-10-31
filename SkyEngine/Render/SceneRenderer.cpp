#include "SEPCH.h"
#include "SceneRenderer.h"

#include "Framebuffer.h"
#include "Renderer.h"
#include "Camera/CameraManager.h"
#include "Core/Application.h"

void CSceneRenderer::Init(TPointer<Scene> InTargetScene, SVector2i InSize)
{	
	SFramebufferSpecification FramebufferSpecification;
	FramebufferSpecification.Size = SVector2ui(InSize);
	Framebuffer = IFramebuffer::Create(FramebufferSpecification);
	TargetScene = InTargetScene;
	ViewSize = InSize;
}

void CSceneRenderer::SizeChanged(SVector2i InSize)
{
	ViewSize = InSize;
	Framebuffer->UpdateSize(SVector2ui(ViewSize));
	// TODO: View matrix update related to specific camera
	CameraManager::GetInstance()->SwitchProjection(EProjectionMode::Perspective);
}

void CSceneRenderer::Render()
{
	Framebuffer->Bind();
	
	GetGraphicsAPI()->Clear(ClearColour);
	GetGraphicsAPI()->SetRenderViewportSize(ViewSize);
	TargetScene->RenderScene();
	
	Framebuffer->Unbind();
		
	// TODO: Shouldn't be here
	// glClearColor(0.9f, 0.4f, 0.4f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT);
}

void CSceneRenderer::SetSceneTarget(TPointer<Scene> InTargetScene)
{
	TargetScene = InTargetScene;
	// TODO: Update camera
}
