#include "SEPCH.h"
#include "SceneRenderer.h"

#include "Framebuffer.h"
#include "Renderer.h"
#include "Core/Application.h"
#include "Entity/Camera.h"

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
	// Update projection - Redundant until update is moved out of get
	GetProjection();
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

void CSceneRenderer::LinkCamera(TPointer<Camera> Camera)
{
	LinkedCamera = Camera;
}

void CSceneRenderer::UpdateOrthographicProjection(float Scale, float MaxViewClipping)
{
	const float HalfWidth = static_cast<float>(ViewSize.X) / Scale;
	const float HalfHeight = static_cast<float>(ViewSize.Y) / Scale;
	//projection = glm::ortho(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, 0.1f, fMaxViewClipping);
	OrthoProjInfo ProjectionInfo;
	ProjectionInfo.Left = -HalfWidth;
	ProjectionInfo.Right = HalfWidth;
	ProjectionInfo.Bottom = -HalfHeight;
	ProjectionInfo.Top = HalfHeight;
	ProjectionInfo.ZNear = 0.1f;
	ProjectionInfo.ZFar = MaxViewClipping;
	Projection.SetOrthographicProjection(ProjectionInfo);
}

void CSceneRenderer::UpdatePerspectiveProjection(float FOV, float MaxViewClipping)
{
	PersProjInfo ProjectionInfo;
	ProjectionInfo.FOV = FOV;
	ProjectionInfo.Width = static_cast<float>(ViewSize.X);
	ProjectionInfo.Height = static_cast<float>(ViewSize.Y);
	ProjectionInfo.zNear = 0.1f;
	ProjectionInfo.zFar = MaxViewClipping;
	Projection.SetPerspectiveProjection(ProjectionInfo);
	// projection = glm::perspective(45.0f, static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, fMaxViewClipping);
}

Matrix4 CSceneRenderer::GetView()
{
	if (LinkedCamera)
	{
		const SVector Forward = LinkedCamera->Transform.Rotation.ToVector();
		const SVector Target = LinkedCamera->Transform.Position + Forward;
		View.SetLookAt(LinkedCamera->Transform.Position,
		               Target,
		               SVector(0.0f, 1.0f, 0.0f));
	}
	return View;
}

Matrix4 CSceneRenderer::GetProjection()
{
	// TODO: Change to event when camera properties change or update?
	if (LinkedCamera)
	{
		switch (LinkedCamera->ProjectionMode)
		{
		case EProjectionMode::Orthographic:
			UpdateOrthographicProjection(LinkedCamera->WindowScale, LinkedCamera->MaxViewClipping);
			break;
		case EProjectionMode::Perspective:
			UpdatePerspectiveProjection(LinkedCamera->FOV, LinkedCamera->MaxViewClipping);
			break;
		}
	}
	return Projection;
}
