// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "TransformationWidget.h"

// Engine Includes //
#include <Camera/CameraManager.h>
#include <Input/Input.h>
#include <Render/Meshes/Basic/Cube.h>
#include <Scene/Scene.h>

#include "Render/Materials/Material.h"
#include "Render/Shaders/ShaderManager.h"
#include "Render/Shaders/UnlitShader.h"

TransformationWidget::TransformationWidget(STransform _Transform, Scene* OwningScene)
: Entity(_Transform, EANCHOR::CENTER)
{	
	XMoveTransform = std::make_shared<Entity>(Transform, EANCHOR::CENTER);
	YMoveTransform = std::make_shared<Entity>(Transform, EANCHOR::CENTER);
	ZMoveTransform = std::make_shared<Entity>(Transform, EANCHOR::CENTER);
	OwningScene->AddEntity(XMoveTransform, true);
	OwningScene->AddEntity(YMoveTransform, true);
	OwningScene->AddEntity(ZMoveTransform, true);
}

TransformationWidget::~TransformationWidget()
{
}

void TransformationWidget::CreateWidgets()
{
	// TODO: Convert to components
	XColour = {0.6f, 0.1f, 0.1f, 1.0f};
	
	const TPointer<CMaterial_Unlit> RedUnlitMaterial = std::make_shared<CMaterial_Unlit>("RedUnlitMaterial");
	RedUnlitMaterial->Params.DiffuseColour = XColour;
	RedUnlitMaterial->bDepthTest = false;
	const TPointer<CMaterial_Unlit> GreenUnlitMaterial = std::make_shared<CMaterial_Unlit>("GreenUnlitMaterial");
	GreenUnlitMaterial->Params.DiffuseColour = YColour;
	GreenUnlitMaterial->bDepthTest = false;
	const TPointer<CMaterial_Unlit> BlueUnlitMaterial = std::make_shared<CMaterial_Unlit>("BlueUnlitMaterial");
	BlueUnlitMaterial->Params.DiffuseColour = ZColour;
	BlueUnlitMaterial->bDepthTest = false;
	
	TPointer<CCube> XMoveTransformMesh = std::make_shared<CCube>(XMoveTransform, 1.0f, 0.1f, 0.1f, RedUnlitMaterial);
	TPointer<CCube> YMoveTransformMesh = std::make_shared<CCube>(YMoveTransform, 0.1f, 1.0f, 0.1f, GreenUnlitMaterial);
	TPointer<CCube> ZMoveTransformMesh = std::make_shared<CCube>(ZMoveTransform, 0.1f, 0.1f, 1.0f, BlueUnlitMaterial);

	// XMoveTransformMesh->MeshMaterial->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};
	// ZMoveTransformMesh->MeshMaterial->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};
	// YMoveTransformMesh->MeshMaterial->StencilColour = {1.0f, 1.0f, 1.0f, 0.7f};

	// TODO: Switch to components in this entity
	XMoveTransform->AddMesh(XMoveTransformMesh);
	YMoveTransform->AddMesh(YMoveTransformMesh);
	ZMoveTransform->AddMesh(ZMoveTransformMesh);

	XMoveTransform->bRayCast = false;
	YMoveTransform->bRayCast = false;
	ZMoveTransform->bRayCast = false;

	//XMoveTransform->bStencil = true;
	//EntityMesh = XMoveTransform;
	//AddMesh(XMoveTransform);
}

void TransformationWidget::Update()
{
	Entity::Update();

	// TODO: Add general bounds check first

	// Clear states
	XHovered = false;
	YHovered = false;
	ZHovered = false;	

	// TODO:
	// const bool bMouseBegin = CInput::GetInstance()->MouseState[CInput::MOUSE_LEFT] == CInput::INPUT_FIRST_PRESS;
	//
	// // Check for hit
	// const SVector RayStart = CameraManager::GetInstance()->GetCameraPosition();
	// const SVector RayDirection = CameraManager::GetInstance()->ScreenToWorldDirection(CInput::GetInstance()->MousePos);
	// SVector HitPos;
	// // TODO: Change hover to lighter colour
	// // TODO: Allow for multi axis (use plane/triangle check between axis)
	// if (XMoveTransform->CheckHit(RayStart, RayDirection, HitPos))
	// {
	// 	if (bMouseBegin)
	// 	{
	// 		XHeld = true;
	// 	}
	// 	XHovered = true;
	// 	HitPosition = HitPos;
	// }
	// else if (YMoveTransform->CheckHit(RayStart, RayDirection, HitPos))
	// {
	// 	if (bMouseBegin)
	// 	{
	// 		YHeld = true;
	// 	}
	// 	YHovered = true;
	// 	HitPosition = HitPos;
	// }
	// else if (ZMoveTransform->CheckHit(RayStart, RayDirection, HitPos))
	// {
	// 	if (bMouseBegin)
	// 	{
	// 		ZHeld = true;
	// 	}
	// 	ZHovered = true;
	// 	HitPosition = HitPos;
	// }
	//
	// if (bMouseBegin)
	// {
	// 	// TODO: Store initial hit offset
	// 	if (XHeld)
	// 	{
	// 		const SVector HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
	// 		GrabOffset = Transform.Position.X - HitPoint.X;
	// 	}
	// 	else if (YHeld)
	// 	{
	// 		const SVector HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 0.0f, 1.0f));
	// 		GrabOffset = Transform.Position.Y - HitPoint.Y;
	// 	}
	// 	else if (ZHeld)
	// 	{
	// 		const SVector HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
	// 		GrabOffset = Transform.Position.Z - HitPoint.Z;
	// 	}
	// }
	// else if (CInput::GetInstance()->MouseState[CInput::MOUSE_LEFT] == CInput::INPUT_FIRST_RELEASE)
	// {
	// 	XHeld = false;
	// 	YHeld = false;
	// 	ZHeld = false;
	// }
	//
	// if (CInput::GetInstance()->MouseState[CInput::MOUSE_LEFT] == CInput::INPUT_HOLD && SelectedEntity)
	// {
	// 	if (XHeld)
	// 	{
	// 		const SVector HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
	// 		Transform.Position.X = HitPoint.X + GrabOffset;
	// 	}
	// 	else if (YHeld)
	// 	{
	// 		const SVector HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 0.0f, 1.0f));
	// 		Transform.Position.Y = HitPoint.Y + GrabOffset;
	// 	}
	// 	else if (ZHeld)
	// 	{
	// 		const SVector HitPoint = Utils::LinePlaneIntersect(RayStart, RayDirection, Transform.Position, glm::vec3(0.0f, 1.0f, 0.0f));
	// 		Transform.Position.Z = HitPoint.Z + GrabOffset;
	// 	}
	// 	SelectedEntity->Transform.Position = Transform.Position;
	// }

	// TODO: Update so colour gets updated (only happens on bind
	// XMoveTransform->EntityMesh->MeshMaterial->bStencil = XHovered;
	// XMoveTransform->EntityMesh->MeshMaterial->Colour = XColour * (XHovered ? 2.0f : 1.0f);
	//
	// YMoveTransform->EntityMesh->MeshMaterial->bStencil = YHovered;
	// YMoveTransform->EntityMesh->MeshMaterial->Colour = YColour * (YHovered ? 2.0f : 1.0f);
	//
	// ZMoveTransform->EntityMesh->MeshMaterial->bStencil = ZHovered;
	// ZMoveTransform->EntityMesh->MeshMaterial->Colour = ZColour * (ZHovered ? 2.0f : 1.0f);

	// Update child transforms
	// TODO: Remove need for component transform system
	XMoveTransform->Transform.Position = Transform.Position + glm::vec3(XMoveTransform->EntityMesh->m_fWidth / 2.0f, 0, 0);
	YMoveTransform->Transform.Position = Transform.Position + glm::vec3(0, YMoveTransform->EntityMesh->m_fHeight / 2.0f, 0);
	ZMoveTransform->Transform.Position = Transform.Position + glm::vec3(0, 0, ZMoveTransform->EntityMesh->m_fDepth / 2.0f);
}

std::vector<TPointer<Entity>> TransformationWidget::GetAdditionalEntitiesToRender()
{
	return {};//{XMoveTransform, YMoveTransform, ZMoveTransform};
}