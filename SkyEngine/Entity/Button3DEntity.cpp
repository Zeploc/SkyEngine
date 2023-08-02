// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Button3DEntity.h"

// Engine Includes //
#include "Camera/CameraManager.h"
#include "Input/Input.h"
#include "Render/Meshes/Basic/Cube.h"

// Static Variables //
bool Button3DEntity::bButtonPressedThisFrame = false;

Button3DEntity::Button3DEntity(STransform _Transform, EANCHOR _Anchor, float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, glm::vec4 _HightlightColour, void (*func)()) : Entity(_Transform, _Anchor)
{
	// TODO: Link UI pointer and set colour?
	TPointer<CMaterial> UIMaterial;
	TPointer<CCube> ButtonCubeMesh = std::make_shared<CCube>(CCube(shared_from_this(), fWidth, fHeight, fDepth, UIMaterial));
	EntityMesh = ButtonCubeMesh;
	btnColour = _Colour;
	btnHighlightColour = _HightlightColour;
}

Button3DEntity::Button3DEntity(STransform _Transform, EANCHOR _Anchor, float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, glm::vec4 _HightlightColour, const char* Texturepath, void (*func)()) : Entity(_Transform, _Anchor)
{
	// TODO: Link UI pointer and set colour?
	TPointer<CMaterial> UIMaterial; // _Colour, Texturepath
	TPointer<CCube> ButtonCubeMesh = std::make_shared<CCube>(CCube(shared_from_this(), fWidth, fHeight, fDepth, UIMaterial));
	EntityMesh = ButtonCubeMesh;
	btnColour = _Colour;
	btnHighlightColour = _HightlightColour;
}

Button3DEntity::~Button3DEntity()
{
}

void Button3DEntity::Update()
{
	if (!bActive)
	{
		return;
	}
	glm::vec3 HalfDimensionvec = glm::vec3(EntityMesh->m_fWidth / 2.0f, EntityMesh->m_fHeight / 2.0f, EntityMesh->m_fDepth / 2.0f);
	bool bHit = CheckHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z))
		|| CheckHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, -HalfDimensionvec.z))
		|| CheckHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, -HalfDimensionvec.y, HalfDimensionvec.z))
		|| CheckHit(glm::vec3(-HalfDimensionvec.x, HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z))
		|| CheckHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(-HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z))
		|| CheckHit(glm::vec3(HalfDimensionvec.x, -HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z));

	if (bHit)
	{
		if (EntityMesh->MeshMaterial)
		{
			// TODO: Link stencil
			// EntityMesh->MeshMaterial->bStencil = true;
		}
		if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_PRESS && !bButtonPressedThisFrame)
		{
			bPressed = true;
			// TODO: Switch to order/depth system
			bButtonPressedThisFrame = true;
			PressDelegate.Broadcast();
		}
		if (EntityMesh->MeshMaterial)
		{
			// TODO: Link runtime colour
			// EntityMesh->MeshMaterial->Colour = btnHighlightColour;
		}
	}
	else
	{
		if (EntityMesh->MeshMaterial)
		{
			// TODO: Link stencil and runtime colour
			// EntityMesh->MeshMaterial->Colour = btnColour;
			// EntityMesh->MeshMaterial->bStencil = false;
		}
	}
}

bool Button3DEntity::CheckHit(glm::vec3 BottomLeftOffset, glm::vec3 TopRightOffset)
{
	SVector HitPos;
	return Utils::CheckFaceHit(BottomLeftOffset,
		TopRightOffset,
		CameraManager::GetInstance()->GetCameraPosition(),
		CameraManager::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos),

		this->shared_from_this(), 
		HitPos);
	
	// glm::vec3 rayDirection = CameraManager::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos);
	//
	// glm::vec3 AnchoredPosition = Utils::GetAncoredPosition(Transform.Position, glm::vec3(EntityMesh->m_fWidth, EntityMesh->m_fHeight, EntityMesh->m_fDepth), EntityAnchor);
	// glm::vec3 lb = BottomLeftOffset + AnchoredPosition;
	// glm::vec3 rt = TopRightOffset + AnchoredPosition;
	// lb = glm::vec4(lb, 1.0f) * GetModel();
	// rt = glm::vec4(rt, 1.0f) * GetModel();
	//
	// glm::vec3 DirFrac = 1.0f / rayDirection;
	//
	// float t1 = (lb.x - CameraManager::GetInstance()->GetCameraPosition().X) * DirFrac.x;
	// float t2 = (rt.x - CameraManager::GetInstance()->GetCameraPosition().X) * DirFrac.x;
	// float t3 = (lb.y - CameraManager::GetInstance()->GetCameraPosition().Y) * DirFrac.y;
	// float t4 = (rt.y - CameraManager::GetInstance()->GetCameraPosition().Y) * DirFrac.y;
	// float t5 = (lb.z - CameraManager::GetInstance()->GetCameraPosition().Z) * DirFrac.z;
	// float t6 = (rt.z - CameraManager::GetInstance()->GetCameraPosition().Z) * DirFrac.z;
	//
	// float tmin = std::fmax(std::fmax(std::fmin(t1, t2), std::fmin(t3, t4)), std::fmin(t5, t6));
	// float tmax = std::fmin(std::fmin(std::fmax(t1, t2), std::fmax(t3, t4)), std::fmax(t5, t6));
	// float t;
	//
	// // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	// if (tmax < 0)
	// {
	// 	t = tmax;
	// }
	// // if tmin > tmax, ray doesn't intersect AABB
	// else if (tmin > tmax)
	// {
	// 	t = tmax;
	// }
	// else
	// {
	// 	t = tmin;
	// 	return true;
	// }
	//
	// return false;
}
