// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "TransformationWidget.h"

// Engine Includes //
#include <Camera/CameraManager.h>
#include <Input/Input.h>
#include <Render/Cube.h>
#include <Scene/Scene.h>

TransformationWidget::TransformationWidget(FTransform _Transform, Scene* OwningScene) : Entity(_Transform, EANCHOR::CENTER)
{
	// TODO: Convert to components
	XMoveTransform = std::make_shared<Entity>(_Transform, EANCHOR::CENTER);
	YMoveTransform = std::make_shared<Entity>(_Transform, EANCHOR::CENTER);
	ZMoveTransform = std::make_shared<Entity>(_Transform, EANCHOR::CENTER);
	XColour = {0.6f, 0.1f, 0.1f, 1.0f};
	std::shared_ptr<Cube> XMoveTransformMesh(new Cube(1.0f, 0.1f, 0.1f, XColour.ToGLM()));
	std::shared_ptr<Cube> ZMoveTransformMesh(new Cube(0.1f, 0.1f, 1.0f, ZColour.ToGLM()));
	std::shared_ptr<Cube> YMoveTransformMesh(new Cube(0.1f, 1.0f, 0.1f, YColour.ToGLM()));
	XMoveTransformMesh->bDepthTest = false;
	ZMoveTransformMesh->bDepthTest = false;
	YMoveTransformMesh->bDepthTest = false;

	XMoveTransformMesh->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};
	ZMoveTransformMesh->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};
	YMoveTransformMesh->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};

	XMoveTransformMesh->SetLit(false);
	ZMoveTransformMesh->SetLit(false);
	YMoveTransformMesh->SetLit(false);

	XMoveTransform->AddMesh(XMoveTransformMesh);
	YMoveTransform->AddMesh(YMoveTransformMesh);
	ZMoveTransform->AddMesh(ZMoveTransformMesh);

	OwningScene->AddEntity(XMoveTransform, true);
	OwningScene->AddEntity(YMoveTransform, true);
	OwningScene->AddEntity(ZMoveTransform, true);
	XMoveTransform->bRayCast = false;
	YMoveTransform->bRayCast = false;
	ZMoveTransform->bRayCast = false;

	//XMoveTransform->bStencil = true;
	//EntityMesh = XMoveTransform;
	//AddMesh(XMoveTransform);
}

TransformationWidget::~TransformationWidget()
{
}

void TransformationWidget::Update()
{
	Entity::Update();

	// TODO: Add general bounds check first

	// Clear states
	XHovered = false;
	YHovered = false;
	ZHovered = false;	

	const bool bMouseBegin = Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS;

	// Check for hit
	const Vector3 RayStart = CameraManager::GetInstance()->GetCameraPosition();
	const Vector3 RayDirection = CameraManager::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos);
	Vector3 HitPos;
	// TODO: Change hover to lighter colour
	// TODO: Allow for multi axis (use plane/triangle check between axis)
	if (XMoveTransform->CheckHit(RayStart, RayDirection, HitPos))
	{
		if (bMouseBegin)
		{
			XHeld = true;
		}
		XHovered = true;
		HitPosition = HitPos;
	}
	else if (YMoveTransform->CheckHit(RayStart, RayDirection, HitPos))
	{
		if (bMouseBegin)
		{
			YHeld = true;
		}
		YHovered = true;
		HitPosition = HitPos;
	}
	else if (ZMoveTransform->CheckHit(RayStart, RayDirection, HitPos))
	{
		if (bMouseBegin)
		{
			ZHeld = true;
		}
		ZHovered = true;
		HitPosition = HitPos;
	}

	if (bMouseBegin)
	{
		// TODO: Store initial hit offset
		if (XHeld)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
			GrabOffset = Transform.Position.X - HitPoint.X;
		}
		else if (YHeld)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 0.0f, 1.0f));
			GrabOffset = Transform.Position.Y - HitPoint.Y;
		}
		else if (ZHeld)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
			GrabOffset = Transform.Position.Z - HitPoint.Z;
		}
	}
	else if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_RELEASE)
	{
		XHeld = false;
		YHeld = false;
		ZHeld = false;
	}

	if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_HOLD && SelectedEntity)
	{
		if (XHeld)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
			Transform.Position.X = HitPoint.X + GrabOffset;
		}
		else if (YHeld)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 0.0f, 1.0f));
			Transform.Position.Y = HitPoint.Y + GrabOffset;
		}
		else if (ZHeld)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
			Transform.Position.Z = HitPoint.Z + GrabOffset;
		}
		SelectedEntity->Transform.Position = Transform.Position;
	}

	// TODO: Update so colour gets updated (only happens on bind
	XMoveTransform->EntityMesh->bStencil = XHovered;
	XMoveTransform->EntityMesh->Colour = XColour.ToGLM() * (XHovered ? 2.0f : 1.0f);
	//XMoveTransform->EntityMesh->Rebind();

	YMoveTransform->EntityMesh->bStencil = YHovered;
	YMoveTransform->EntityMesh->Colour = YColour.ToGLM() * (YHovered ? 2.0f : 1.0f);
	//YMoveTransform->EntityMesh->Rebind();

	ZMoveTransform->EntityMesh->bStencil = ZHovered;
	ZMoveTransform->EntityMesh->Colour = ZColour.ToGLM() * (ZHovered ? 2.0f : 1.0f);
	//ZMoveTransform->EntityMesh->Rebind();

	// Update child transforms
	// TODO: Remove need for component transform system
	XMoveTransform->Transform.Position = Transform.Position + glm::vec3(XMoveTransform->EntityMesh->m_fWidth / 2.0f, 0, 0);
	YMoveTransform->Transform.Position = Transform.Position + glm::vec3(0, YMoveTransform->EntityMesh->m_fHeight / 2.0f, 0);
	ZMoveTransform->Transform.Position = Transform.Position + glm::vec3(0, 0, ZMoveTransform->EntityMesh->m_fDepth / 2.0f);
}

void TransformationWidget::DrawEntity()
{
	//Entity::DrawEntity();
	if (!bVisible)
	{
		return;
	}
}
