// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "TransformationWidget.h"

// Engine Includes //
#include "Engine/Camera/Camera.h"
#include "Engine/Input/Input.h"
#include "Engine/Render/Cube.h"
#include "Engine/Scene/Scene.h"

TransformationWidget::TransformationWidget(FTransform _Transform, Scene* OwningScene) : Entity(_Transform, EANCHOR::CENTER)
{
	XMoveTransform = std::make_shared<Entity>(Entity(_Transform, EANCHOR::CENTER));
	YMoveTransform = std::make_shared<Entity>(Entity(_Transform, EANCHOR::CENTER));
	ZMoveTransform = std::make_shared<Entity>(Entity(_Transform, EANCHOR::CENTER));
	std::shared_ptr<Cube> XMoveTransformMesh = std::make_shared<Cube>(Cube(1.0f, 0.1f, 0.1f, {0.9f, 0.1f, 0.1f, 1.0f}));
	std::shared_ptr<Cube> ZMoveTransformMesh = std::make_shared<Cube>(Cube(0.1f, 0.1f, 1.0f, {0.1f, 0.9f, 0.1f, 1.0f}));
	std::shared_ptr<Cube> YMoveTransformMesh = std::make_shared<Cube>(Cube(0.1f, 1.0f, 0.1f, {0.1f, 0.1f, 0.9f, 1.0f}));
	XMoveTransformMesh->bDepthTest = false;
	ZMoveTransformMesh->bDepthTest = false;
	YMoveTransformMesh->bDepthTest = false;

	XMoveTransformMesh->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};
	ZMoveTransformMesh->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};
	YMoveTransformMesh->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};

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
	if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS)
	{
		Vector3 rayStart = Camera::GetInstance()->GetCameraPosition();
		Vector3 rayDirection = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos);
		Vector3 HitPos;
		if (Utils::CheckHit(rayStart, rayDirection, XMoveTransform, HitPos))
		{
			XHit = true;
			XMoveTransform->EntityMesh->bStencil = true;
			HitPosition = HitPos;
		}
		if (Utils::CheckHit(rayStart, rayDirection, YMoveTransform, HitPos))
		{
			YHit = true;
			YMoveTransform->EntityMesh->bStencil = true;
			HitPosition = HitPos;
		}
		if (Utils::CheckHit(rayStart, rayDirection, ZMoveTransform, HitPos))
		{
			ZHit = true;
			ZMoveTransform->EntityMesh->bStencil = true;
			HitPosition = HitPos;
		}
	}
	else if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_RELEASE)
	{
		XMoveTransform->EntityMesh->bStencil = false;
		YMoveTransform->EntityMesh->bStencil = false;
		ZMoveTransform->EntityMesh->bStencil = false;
		XHit = false;
		YHit = false;
		ZHit = false;
	}

	if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_HOLD && SelectedEntity)
	{
		const Vector3 RayStart = Camera::GetInstance()->GetCameraPosition();
		const Vector3 RayDirection = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos);

		if (XHit)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
			Transform.Position.X = HitPoint.X;
		}
		else if (YHit)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 0.0f, 1.0f));
			Transform.Position.Y = HitPoint.Y;
		}
		else if (ZHit)
		{
			const Vector3 HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
			Transform.Position.Z = HitPoint.Z;
		}
		SelectedEntity->Transform.Position = Transform.Position;
	}
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
