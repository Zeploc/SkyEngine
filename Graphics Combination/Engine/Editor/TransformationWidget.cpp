// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "TransformationWidget.h"

// Engine Includes //
#include "Engine/Camera/Camera.h"
#include "Engine/Input/Input.h"
#include "Engine/Render/Cube.h"
#include "Engine/Scene/Scene.h"

TransformationWidget::TransformationWidget(Utils::Transform _Transform, Scene* OwningScene) : Entity(_Transform, Utils::CENTER)
{
	XMoveTransform = std::make_shared<Entity>(Entity(_Transform, Utils::CENTER));
	YMoveTransform = std::make_shared<Entity>(Entity(_Transform, Utils::CENTER));
	ZMoveTransform = std::make_shared<Entity>(Entity(_Transform, Utils::CENTER));
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
		glm::vec3 rayStart = Camera::GetInstance()->GetCameraPosition();
		glm::vec3 rayDirection = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos);
		glm::vec3 HitPos;
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
		glm::vec3 rayStart = Camera::GetInstance()->GetCameraPosition();
		glm::vec3 rayDirection = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos);

		if (XHit)
		{
			glm::vec3 HitPoint = Utils::LinePlaneIntersect(rayStart, rayDirection, transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
			transform.Position.x = HitPoint.x;
		}
		else if (YHit)
		{
			glm::vec3 HitPoint = Utils::LinePlaneIntersect(rayStart, rayDirection, transform.Position, glm::vec3(0.0f, 0.0f, 1.0f));
			transform.Position.y = HitPoint.y;
		}
		else if (ZHit)
		{
			glm::vec3 HitPoint = Utils::LinePlaneIntersect(rayStart, rayDirection, transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
			transform.Position.z = HitPoint.z;
		}
		SelectedEntity->transform.Position = transform.Position;
	}
	XMoveTransform->transform.Position = transform.Position + glm::vec3(XMoveTransform->EntityMesh->m_fWidth / 2.0f, 0, 0);
	YMoveTransform->transform.Position = transform.Position + glm::vec3(0, YMoveTransform->EntityMesh->m_fHeight / 2.0f, 0);
	ZMoveTransform->transform.Position = transform.Position + glm::vec3(0, 0, ZMoveTransform->EntityMesh->m_fDepth / 2.0f);
}

void TransformationWidget::DrawEntity()
{
	//Entity::DrawEntity();
	if (!bVisible)
	{
		return;
	}
}
