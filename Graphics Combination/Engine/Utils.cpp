//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    CSprite.cpp
// Description    	:    main implementation for CSprite
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <vector>

// OpenGL Library Includes //
#include <glew.h>
//#include <freeglut.h>

// Engine Includes //
#include "Entity.h"
#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cube.h"
#include "LogManager.h"

// Local Includes //

// This Includes //
#include "Utils.h"

// Static Variables //
int Utils::iEntityNumber = 0;
std::shared_ptr<Entity> Utils::WorldCubeMap;

/************************************************************
#--Description--#:  Gets position from anchor and current position
#--Author--#: 		Alex Coultas
#--Parameters--#:	Element position, dimensions and current anchor
#--Return--#: 		Returns vector 3 of new position
************************************************************/
glm::vec3 Utils::GetAncoredPosition2D(glm::vec2 position, glm::vec2 Dimensions, EANCHOR _AnchorType)
{
	glm::vec3 NewPos;
	switch (_AnchorType)
	{
	case Utils::TOP_LEFT:
		NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y - Dimensions.y / 2, 0);
		break;
	case Utils::TOP_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y - Dimensions.y / 2, 0);
		break;
	case Utils::TOP_CENTER:
		NewPos = glm::vec3(position.x, position.y - Dimensions.y / 2, 0);
		break;
	case Utils::CENTER_LEFT:
		NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y, 0);
		break;
	case Utils::CENTER:
		NewPos = glm::vec3(position.x, position.y, 0);
		break;
	case Utils::CENTER_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y, 0);
		break;
	case Utils::BOTTOM_LEFT:
		NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y + Dimensions.y / 2, 0);
		break;
	case Utils::BOTTOM_CENTER:
		NewPos = glm::vec3(position.x, position.y + Dimensions.y / 2, 0);
		break;
	case Utils::BOTTOM_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y + Dimensions.y / 2, 0);
		break;
	default:
		break;
	}
	return NewPos;
}

/************************************************************
#--Description--#:  Gets position from anchor and current position
#--Author--#: 		Alex Coultas
#--Parameters--#:	Element position, dimensions and current anchor
#--Return--#: 		Returns vector 3 of new position
************************************************************/
glm::vec3 Utils::GetAncoredPosition(glm::vec3 position, glm::vec3 Dimensions, EANCHOR _AnchorType)
{
	glm::vec3 NewPos;
	switch (_AnchorType)
	{
	case Utils::TOP_LEFT:
		NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y - Dimensions.y / 2, position.z);
		break;
	case Utils::TOP_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y - Dimensions.y / 2, position.z);
		break;
	case Utils::TOP_CENTER:
		NewPos = glm::vec3(position.x, position.y - Dimensions.y / 2, position.z);
		break;
	case Utils::CENTER_LEFT:
		NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y, position.z);
		break;
	case Utils::CENTER:
		NewPos = glm::vec3(position.x, position.y, position.z);
		break;
	case Utils::CENTER_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y, position.z);
		break;
	case Utils::BOTTOM_LEFT:
		NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y + Dimensions.y / 2, position.z);
		break;
	case Utils::BOTTOM_CENTER:
		NewPos = glm::vec3(position.x, position.y + Dimensions.y / 2, position.z);
		break;
	case Utils::BOTTOM_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y + Dimensions.y / 2, position.z);
		break;
	default:
		break;
	}
	return NewPos;
}

/************************************************************
#--Description--#:  Gets text position from anchor and current position
#--Author--#: 		Alex Coultas
#--Parameters--#:	Element position, dimensions and current anchor
#--Return--#: 		Returns vector 3 of new position
************************************************************/
glm::vec3 Utils::GetTextAncoredPosition(glm::vec2 position, glm::vec2 Dimensions, EANCHOR _AnchorType)
{
	glm::vec3 NewPos;
	switch (_AnchorType)
	{
	case Utils::TOP_LEFT:
		NewPos = glm::vec3(position.x, position.y + Dimensions.y / 2, 0);
		break;
	case Utils::TOP_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x, position.y + Dimensions.y / 2, 0);
		break;
	case Utils::TOP_CENTER:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y + Dimensions.y / 2, 0);
		break;
	case Utils::CENTER_LEFT:
		NewPos = glm::vec3(position.x, position.y, 0);
		break;
	case Utils::CENTER:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y, 0);
		break;
	case Utils::CENTER_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x, position.y, 0);
		break;
	case Utils::BOTTOM_LEFT:
		NewPos = glm::vec3(position.x, position.y - Dimensions.y / 2, 0);
		break;
	case Utils::BOTTOM_CENTER:
		NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y - Dimensions.y / 2, 0);
		break;
	case Utils::BOTTOM_RIGHT:
		NewPos = glm::vec3(position.x - Dimensions.x, position.y - Dimensions.y / 2, 0);
		break;
	default:
		break;
	}
	return NewPos;
}

/************************************************************
#--Description--#:  Returns whether the object is colliding
#--Author--#: 		Alex Coultas
#--Parameters--#:	two pointers to the entities
#--Return--#: 		Returns boolean whether the entities are colliding
************************************************************/
bool Utils::isColliding2D(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2)
{
	std::shared_ptr<Plane> Entity1Mesh = std::dynamic_pointer_cast<Plane>(Entity1->EntityMesh);
	std::shared_ptr<Plane> Entity2Mesh = std::dynamic_pointer_cast<Plane>(Entity2->EntityMesh);
	float HalfWidth1 = (Entity1Mesh->CollisionBox.fWidth / 2) * abs(Entity1->transform.Scale.x);
	float HalfHeight1 = (Entity1Mesh->CollisionBox.fHeight / 2) * abs(Entity1->transform.Scale.y);
	float HalfWidth2 = (Entity2Mesh->CollisionBox.fWidth / 2) * abs(Entity2->transform.Scale.x);
	float HalfHeight2 = (Entity2Mesh->CollisionBox.fHeight / 2) * abs(Entity2->transform.Scale.y);
	
	glm::vec3 Entity1Pos = GetAncoredPosition2D(Entity1->transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * (glm::vec2)Entity1->transform.Scale, Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = GetAncoredPosition2D(Entity2->transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * (glm::vec2)Entity2->transform.Scale, Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x + HalfWidth1 > Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x - HalfWidth2
		&& Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x - HalfWidth1 < Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x + HalfWidth2
		&& Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y + HalfHeight1 > Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y - HalfHeight2
		&& Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y - HalfHeight1 < Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y + HalfHeight2)
		return true;
	return false;
}

/************************************************************
#--Description--#:  Returns whether the object would collide given movement
#--Author--#: 		Alex Coultas
#--Parameters--#:	two pointers to the entities and the movement to check for first entity
#--Return--#: 		Returns boolean whether the entities would collide
************************************************************/
bool Utils::CheckCollision2D(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2, glm::vec2 Movement)
{
	std::shared_ptr<Plane> Entity1Mesh = std::dynamic_pointer_cast<Plane>(Entity1->EntityMesh);
	std::shared_ptr<Plane> Entity2Mesh = std::dynamic_pointer_cast<Plane>(Entity2->EntityMesh);
	float HalfWidth1 = (Entity1Mesh->CollisionBox.fWidth / 2) * abs(Entity1->transform.Scale.x);
	float HalfHeight1 = (Entity1Mesh->CollisionBox.fHeight / 2) * abs(Entity1->transform.Scale.y);
	float HalfWidth2 = (Entity2Mesh->CollisionBox.fWidth / 2) * abs(Entity2->transform.Scale.x);
	float HalfHeight2 = (Entity2Mesh->CollisionBox.fHeight / 2) * abs(Entity2->transform.Scale.y);

	glm::vec3 Entity1Pos = GetAncoredPosition2D(Entity1->transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * (glm::vec2)Entity1->transform.Scale, Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = GetAncoredPosition2D(Entity2->transform.Position + glm::vec3(Entity2Mesh->CollisionBox.v2Offset, 0), glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * (glm::vec2)Entity2->transform.Scale, Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x + HalfWidth1 + Movement.x > Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x - HalfWidth2
		&& Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x - HalfWidth1 + Movement.x < Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x + HalfWidth2
		&& Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y + HalfHeight1 + Movement.y > Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y - HalfHeight2
		&& Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y - HalfHeight1 + Movement.y < Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y + HalfHeight2)
		return true;
	return false;
}

/************************************************************
#--Description--#:  Gets the distance from the edges of the entities collision boxes
#--Author--#: 		Alex Coultas
#--Parameters--#:	two pointers to the entities
#--Return--#: 		Returns vector 2 of distance
************************************************************/
glm::vec2 Utils::GetDistance2D(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2)
{
	std::shared_ptr<Plane> Entity1Mesh = std::dynamic_pointer_cast<Plane>(Entity1->EntityMesh);
	std::shared_ptr<Plane> Entity2Mesh = std::dynamic_pointer_cast<Plane>(Entity2->EntityMesh);
	glm::vec2 fDistance = glm::vec2(0, 0);
	float HalfWidth1 = (Entity1Mesh->CollisionBox.fWidth / 2) * abs(Entity1->transform.Scale.x);
	float HalfHeight1 = (Entity1Mesh->CollisionBox.fHeight / 2) * abs(Entity1->transform.Scale.y);
	float HalfWidth2 = (Entity2Mesh->CollisionBox.fWidth / 2) * abs(Entity2->transform.Scale.x);
	float HalfHeight2 = (Entity2Mesh->CollisionBox.fHeight / 2) * abs(Entity2->transform.Scale.y);
	
	glm::vec3 Entity1Pos = GetAncoredPosition2D(Entity1->transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * (glm::vec2)Entity1->transform.Scale, Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = GetAncoredPosition2D(Entity2->transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * (glm::vec2)Entity2->transform.Scale, Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x + HalfWidth1 <= Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x - HalfWidth2)
	{
		fDistance.x = abs((Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x + HalfWidth1) - (Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x - HalfWidth2));
	}
	else if (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x - HalfWidth1 >= Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x + HalfWidth2)
	{
		fDistance.x = abs((Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x - HalfWidth1) - (Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x + HalfWidth2));
	}

	if (Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y + HalfHeight1 <= Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y - HalfHeight2)
	{
		fDistance.y = abs((Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y + HalfHeight1) - (Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y - HalfHeight2));
	}
	else if (Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y - HalfHeight1 >= Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y + HalfHeight2)
	{
		fDistance.y = abs((Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y - HalfHeight1) - (Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y + HalfHeight2));
	}
	return fDistance;
}

/************************************************************
#--Description--#:  Gets the distance with direction from the edges of the entities collision boxes
#--Author--#: 		Alex Coultas
#--Parameters--#:	two pointers to the entities
#--Return--#: 		Returns vector 2 of distance with direction
************************************************************/
glm::vec2 Utils::GetDifference2D(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2)
{
	std::shared_ptr<Plane> Entity1Mesh = std::dynamic_pointer_cast<Plane>(Entity1->EntityMesh);
	std::shared_ptr<Plane> Entity2Mesh = std::dynamic_pointer_cast<Plane>(Entity2->EntityMesh);
	glm::vec2 fDistance = glm::vec2(0, 0);
	float HalfWidth1 = (Entity1Mesh->CollisionBox.fWidth / 2) * abs(Entity1->transform.Scale.x);
	float HalfHeight1 = (Entity1Mesh->CollisionBox.fHeight / 2) * abs(Entity1->transform.Scale.y);
	float HalfWidth2 = (Entity2Mesh->CollisionBox.fWidth / 2) * abs(Entity2->transform.Scale.x);
	float HalfHeight2 = (Entity2Mesh->CollisionBox.fHeight / 2) * abs(Entity2->transform.Scale.y);

	glm::vec3 Entity1Pos = GetAncoredPosition2D(Entity1->transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * (glm::vec2)Entity1->transform.Scale, Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = GetAncoredPosition2D(Entity2->transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * (glm::vec2)Entity2->transform.Scale, Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x + HalfWidth1 < Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x - HalfWidth2)
	{
		fDistance.x = (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x + HalfWidth1) - (Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x - HalfWidth2);
	}
	else if (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x - HalfWidth1 > Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x + HalfWidth2)
	{
		fDistance.x = (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x - HalfWidth1) - (Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x + HalfWidth2);
	}

	if (Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y + HalfHeight1 < Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y - HalfHeight2)
	{
		fDistance.y = (Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y + HalfHeight1) - (Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y - HalfHeight2);
	}
	else if (Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y - HalfHeight1 > Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y + HalfHeight2)
	{
		fDistance.y = (Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y - HalfHeight1) - (Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y + HalfHeight2);
	}
	return fDistance;
}


int Utils::AddEntityID()
{
	iEntityNumber++;
	return iEntityNumber;
}


bool Utils::CheckHit(glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3& HitPos)
{
	if (!EntityCheck->EntityMesh) return false;
	if (EntityCheck->EntityMesh->m_eShape == EMESHTYPE::SPHERE && std::dynamic_pointer_cast<Sphere>(EntityCheck->EntityMesh))
	{
		return CheckSphereEntityHit(RayStart, RayDirection, EntityCheck, HitPos);
	}
	else if (EntityCheck->EntityMesh->m_eShape == EMESHTYPE::CUBE && std::dynamic_pointer_cast<Cube>(EntityCheck->EntityMesh))
	{
		return CheckCubeEntityHit(RayStart, RayDirection, EntityCheck, HitPos);
	}
	else if (EntityCheck->EntityMesh->m_eShape == EMESHTYPE::PLANE && std::dynamic_pointer_cast<Plane>(EntityCheck->EntityMesh))
	{
		return CheckPlaneEntityHit(RayStart, RayDirection, EntityCheck, HitPos);
	}
	//LogManager::GetInstance()->DisplayLogMessage("Could not find mesh type to perform ray hit check!");
	return false;
}


bool Utils::CheckSphereEntityHit(glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3 & HitPos)
{
	glm::vec3 v = EntityCheck->transform.Position - RayStart;

	float a = glm::dot(RayDirection, RayDirection);
	float b = 2.0f * glm::dot(v, RayDirection);
	float c = glm::dot(v, v) - EntityCheck->EntityMesh->m_fWidth * EntityCheck->EntityMesh->m_fWidth;
	float d = b * b - 4 * a * c;

	if (d > 0)
	{
		float x1 = (-b - sqrt(d)) / 2;
		float x2 = (-b + sqrt(d)) / 2;

		float XPos = RayStart.x + RayDirection.x * -x2;
		float YPos = RayStart.y + RayDirection.y * -x2;
		float ZPos = RayStart.z + RayDirection.z * -x2;
		HitPos = { XPos, YPos, ZPos };

		return true;

		//std::cout << "Position hit [" << XPos << ", " << YPos << ", " << ZPos << "]" << std::endl;
		if (x1 >= 0 && x2 >= 0)
		{
			//std::cout << "Ray cast hit!\n";
		}
		else if ((x1 < 0 && x2 >= 0))
		{
			//std::cout << "Ray cast hit from inside!\n";
		}
		else if (abs(x1 - x2) <= 0.3f)
		{
			//std::cout << "Ray cast hitting edge!!\n";
		}
		else
		{
			//std::cout << "Ray cast hit!\n";
		}
	}
	else if (d <= 0)
	{
		//std::cout << "Ray cast missed!\n";
	}
	return false;
}

bool Utils::CheckCubeEntityHit(glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3 & HitPos)
{
	glm::vec3 HalfDimensionvec = glm::vec3(EntityCheck->EntityMesh->m_fWidth / 2.0f, EntityCheck->EntityMesh->m_fHeight / 2.0f, EntityCheck->EntityMesh->m_fDepth / 2.0f);
	std::vector<glm::vec3> HitPositions;
	if (CheckFaceHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, -HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, -HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(glm::vec3(-HalfDimensionvec.x, HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(-HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(glm::vec3(HalfDimensionvec.x, -HalfDimensionvec.y, -HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}

	if (HitPositions.size() <= 0)
		return false;
	HitPos = HitPositions[0];

	if (HitPositions.size() == 1)
		return true;

	for (int i = 0; i < HitPositions.size(); i++)
	{
		if (glm::length(RayStart - HitPositions[i]) < glm::length(RayStart - HitPos))
		{
			HitPos = HitPositions[i];
		}
	}
	return true;
}

bool Utils::CheckPlaneEntityHit(glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3 & HitPos)
{
	glm::vec3 HalfDimensionvec = glm::vec3(EntityCheck->EntityMesh->m_fWidth / 2.0f, EntityCheck->EntityMesh->m_fHeight / 2.0f, EntityCheck->EntityMesh->m_fDepth / 2.0f);
	if (CheckFaceHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		return true;
	}
	return false;
}

glm::vec3 Utils::LinePlaneIntersect(glm::vec3 RayStart, glm::vec3 RayDirection, glm::vec3 PlanePos, glm::vec3 PlaneNormal)
{
	glm::vec3 diff = RayStart - PlanePos;
	double prod1 = glm::dot(diff, PlaneNormal);
	double prod2 = glm::dot(RayDirection, PlaneNormal);
	float prod3 = prod1 / prod2;
	return RayStart - RayDirection * prod3;
}

bool Utils::CheckFaceHit(glm::vec3 BottomLeftOffset, glm::vec3 TopRightOffset, glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3& HitPos)
{
	glm::vec3 AnchoredPosition = Utils::GetAncoredPosition(EntityCheck->transform.Position, glm::vec3(EntityCheck->EntityMesh->m_fWidth, EntityCheck->EntityMesh->m_fHeight, EntityCheck->EntityMesh->m_fDepth), EntityCheck->EntityAnchor);
	glm::vec3 lb = BottomLeftOffset + AnchoredPosition;
	glm::vec3 rt = TopRightOffset + AnchoredPosition;
	lb = glm::vec4(lb, 1.0f) * EntityCheck->GetModel();
	rt = glm::vec4(rt, 1.0f) * EntityCheck->GetModel();

	glm::vec3 DirFrac = 1.0f / RayDirection;

	float t1 = (lb.x - RayStart.x) * DirFrac.x;
	float t2 = (rt.x - RayStart.x) * DirFrac.x;
	float t3 = (lb.y - RayStart.y) * DirFrac.y;
	float t4 = (rt.y - RayStart.y) * DirFrac.y;
	float t5 = (lb.z - RayStart.z) * DirFrac.z;
	float t6 = (rt.z - RayStart.z) * DirFrac.z;

	float tmin = std::fmax(std::fmax(std::fmin(t1, t2), std::fmin(t3, t4)), std::fmin(t5, t6));
	float tmax = std::fmin(std::fmin(std::fmax(t1, t2), std::fmax(t3, t4)), std::fmax(t5, t6));
	float t;

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		t = tmax;
	}
	// if tmin > tmax, ray doesn't intersect AABB
	else if (tmin > tmax)
	{
		t = tmax;
	}
	else
	{
		t = tmin;
		HitPos = RayStart + RayDirection * t;
		return true;
	}
	return false;
}
