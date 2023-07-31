// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Utils.h"

// Library Includes //
#include <sstream>
#include <vector>

// Engine Includes //
#include "LogManager.h"
#include "Entity/Entity.h"
#include "Math/Vector4.h"
#include "Render/Meshes/Basic/Cube.h"
#include "Render/Meshes/Basic/Plane.h"

// Local Includes //

// Static Variables //
int Utils::iEntityNumber = 0;
Pointer<Entity> Utils::WorldCubeMap;

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
		case EANCHOR::TOP_LEFT:
			NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y - Dimensions.y / 2, 0);
			break;
		case EANCHOR::TOP_RIGHT:
			NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y - Dimensions.y / 2, 0);
			break;
		case EANCHOR::TOP_CENTER:
			NewPos = glm::vec3(position.x, position.y - Dimensions.y / 2, 0);
			break;
		case EANCHOR::CENTER_LEFT:
			NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y, 0);
			break;
		case EANCHOR::CENTER:
			NewPos = glm::vec3(position.x, position.y, 0);
			break;
		case EANCHOR::CENTER_RIGHT:
			NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y, 0);
			break;
		case EANCHOR::BOTTOM_LEFT:
			NewPos = glm::vec3(position.x + Dimensions.x / 2, position.y + Dimensions.y / 2, 0);
			break;
		case EANCHOR::BOTTOM_CENTER:
			NewPos = glm::vec3(position.x, position.y + Dimensions.y / 2, 0);
			break;
		case EANCHOR::BOTTOM_RIGHT:
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
glm::vec3 Utils::GetAncoredPosition(Vector3 position, Vector3 Dimensions, EANCHOR _AnchorType)
{
	glm::vec3 NewPos;
	switch (_AnchorType)
	{
		case EANCHOR::TOP_LEFT:
			NewPos = glm::vec3(position.X + Dimensions.X / 2, position.Y - Dimensions.Y / 2, position.Z);
			break;
		case EANCHOR::TOP_RIGHT:
			NewPos = glm::vec3(position.X - Dimensions.X / 2, position.Y - Dimensions.Y / 2, position.Z);
			break;
		case EANCHOR::TOP_CENTER:
			NewPos = glm::vec3(position.X, position.Y - Dimensions.Y / 2, position.Z);
			break;
		case EANCHOR::CENTER_LEFT:
			NewPos = glm::vec3(position.X + Dimensions.X / 2, position.Y, position.Z);
			break;
		case EANCHOR::CENTER:
			NewPos = glm::vec3(position.X, position.Y, position.Z);
			break;
		case EANCHOR::CENTER_RIGHT:
			NewPos = glm::vec3(position.X - Dimensions.X / 2, position.Y, position.Z);
			break;
		case EANCHOR::BOTTOM_LEFT:
			NewPos = glm::vec3(position.X + Dimensions.X / 2, position.Y + Dimensions.Y / 2, position.Z);
			break;
		case EANCHOR::BOTTOM_CENTER:
			NewPos = glm::vec3(position.X, position.Y + Dimensions.Y / 2, position.Z);
			break;
		case EANCHOR::BOTTOM_RIGHT:
			NewPos = glm::vec3(position.X - Dimensions.X / 2, position.Y + Dimensions.Y / 2, position.Z);
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
		case EANCHOR::TOP_LEFT:
			NewPos = glm::vec3(position.x, position.y + Dimensions.y / 2, 0);
			break;
		case EANCHOR::TOP_RIGHT:
			NewPos = glm::vec3(position.x - Dimensions.x, position.y + Dimensions.y / 2, 0);
			break;
		case EANCHOR::TOP_CENTER:
			NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y + Dimensions.y / 2, 0);
			break;
		case EANCHOR::CENTER_LEFT:
			NewPos = glm::vec3(position.x, position.y, 0);
			break;
		case EANCHOR::CENTER:
			NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y, 0);
			break;
		case EANCHOR::CENTER_RIGHT:
			NewPos = glm::vec3(position.x - Dimensions.x, position.y, 0);
			break;
		case EANCHOR::BOTTOM_LEFT:
			NewPos = glm::vec3(position.x, position.y - Dimensions.y / 2, 0);
			break;
		case EANCHOR::BOTTOM_CENTER:
			NewPos = glm::vec3(position.x - Dimensions.x / 2, position.y - Dimensions.y / 2, 0);
			break;
		case EANCHOR::BOTTOM_RIGHT:
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
bool Utils::isColliding2D(Pointer<Entity> Entity1, Pointer<Entity> Entity2)
{
	Pointer<Plane> Entity1Mesh = std::dynamic_pointer_cast<Plane>(Entity1->EntityMesh);
	Pointer<Plane> Entity2Mesh = std::dynamic_pointer_cast<Plane>(Entity2->EntityMesh);
	float HalfWidth1 = (Entity1Mesh->CollisionBox.fWidth / 2) * abs(Entity1->Transform.Scale.X);
	float HalfHeight1 = (Entity1Mesh->CollisionBox.fHeight / 2) * abs(Entity1->Transform.Scale.Y);
	float HalfWidth2 = (Entity2Mesh->CollisionBox.fWidth / 2) * abs(Entity2->Transform.Scale.X);
	float HalfHeight2 = (Entity2Mesh->CollisionBox.fHeight / 2) * abs(Entity2->Transform.Scale.Y);

	glm::vec3 Entity1Pos = GetAncoredPosition2D(Entity1->Transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * static_cast<glm::vec2>(Entity1->Transform.Scale), Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = GetAncoredPosition2D(Entity2->Transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * static_cast<glm::vec2>(Entity2->Transform.Scale), Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x + HalfWidth1 > Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x - HalfWidth2
		&& Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x - HalfWidth1 < Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x + HalfWidth2
		&& Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y + HalfHeight1 > Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y - HalfHeight2
		&& Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y - HalfHeight1 < Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y + HalfHeight2)
	{
		return true;
	}
	return false;
}

/************************************************************
#--Description--#:  Returns whether the object would collide given movement
#--Author--#: 		Alex Coultas
#--Parameters--#:	two pointers to the entities and the movement to check for first entity
#--Return--#: 		Returns boolean whether the entities would collide
************************************************************/
bool Utils::CheckCollision2D(Pointer<Entity> Entity1, Pointer<Entity> Entity2, glm::vec2 Movement)
{
	Pointer<Plane> Entity1Mesh = std::dynamic_pointer_cast<Plane>(Entity1->EntityMesh);
	Pointer<Plane> Entity2Mesh = std::dynamic_pointer_cast<Plane>(Entity2->EntityMesh);
	float HalfWidth1 = (Entity1Mesh->CollisionBox.fWidth / 2) * abs(Entity1->Transform.Scale.X);
	float HalfHeight1 = (Entity1Mesh->CollisionBox.fHeight / 2) * abs(Entity1->Transform.Scale.Y);
	float HalfWidth2 = (Entity2Mesh->CollisionBox.fWidth / 2) * abs(Entity2->Transform.Scale.X);
	float HalfHeight2 = (Entity2Mesh->CollisionBox.fHeight / 2) * abs(Entity2->Transform.Scale.Y);

	glm::vec3 Entity1Pos = GetAncoredPosition2D(Entity1->Transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * static_cast<glm::vec2>(Entity1->Transform.Scale), Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = GetAncoredPosition2D(Entity2->Transform.Position + glm::vec3(Entity2Mesh->CollisionBox.v2Offset, 0), glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * static_cast<glm::vec2>(Entity2->Transform.Scale), Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x + HalfWidth1 + Movement.x > Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x - HalfWidth2
		&& Entity1Pos.x + Entity1Mesh->CollisionBox.v2Offset.x - HalfWidth1 + Movement.x < Entity2Pos.x + Entity2Mesh->CollisionBox.v2Offset.x + HalfWidth2
		&& Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y + HalfHeight1 + Movement.y > Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y - HalfHeight2
		&& Entity1Pos.y + Entity1Mesh->CollisionBox.v2Offset.y - HalfHeight1 + Movement.y < Entity2Pos.y + Entity2Mesh->CollisionBox.v2Offset.y + HalfHeight2)
	{
		return true;
	}
	return false;
}

/************************************************************
#--Description--#:  Gets the distance from the edges of the entities collision boxes
#--Author--#: 		Alex Coultas
#--Parameters--#:	two pointers to the entities
#--Return--#: 		Returns vector 2 of distance
************************************************************/
glm::vec2 Utils::GetDistance2D(Pointer<Entity> Entity1, Pointer<Entity> Entity2)
{
	Pointer<Plane> Entity1Mesh = std::dynamic_pointer_cast<Plane>(Entity1->EntityMesh);
	Pointer<Plane> Entity2Mesh = std::dynamic_pointer_cast<Plane>(Entity2->EntityMesh);
	glm::vec2 fDistance = glm::vec2(0, 0);
	float HalfWidth1 = (Entity1Mesh->CollisionBox.fWidth / 2) * abs(Entity1->Transform.Scale.X);
	float HalfHeight1 = (Entity1Mesh->CollisionBox.fHeight / 2) * abs(Entity1->Transform.Scale.Y);
	float HalfWidth2 = (Entity2Mesh->CollisionBox.fWidth / 2) * abs(Entity2->Transform.Scale.X);
	float HalfHeight2 = (Entity2Mesh->CollisionBox.fHeight / 2) * abs(Entity2->Transform.Scale.Y);

	glm::vec3 Entity1Pos = GetAncoredPosition2D(Entity1->Transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * static_cast<glm::vec2>(Entity1->Transform.Scale), Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = GetAncoredPosition2D(Entity2->Transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * static_cast<glm::vec2>(Entity2->Transform.Scale), Entity2->EntityAnchor);

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
glm::vec2 Utils::GetDifference2D(Pointer<Entity> Entity1, Pointer<Entity> Entity2)
{
	Pointer<Plane> Entity1Mesh = std::dynamic_pointer_cast<Plane>(Entity1->EntityMesh);
	Pointer<Plane> Entity2Mesh = std::dynamic_pointer_cast<Plane>(Entity2->EntityMesh);
	glm::vec2 fDistance = glm::vec2(0, 0);
	float HalfWidth1 = (Entity1Mesh->CollisionBox.fWidth / 2) * abs(Entity1->Transform.Scale.X);
	float HalfHeight1 = (Entity1Mesh->CollisionBox.fHeight / 2) * abs(Entity1->Transform.Scale.Y);
	float HalfWidth2 = (Entity2Mesh->CollisionBox.fWidth / 2) * abs(Entity2->Transform.Scale.X);
	float HalfHeight2 = (Entity2Mesh->CollisionBox.fHeight / 2) * abs(Entity2->Transform.Scale.Y);

	glm::vec3 Entity1Pos = GetAncoredPosition2D(Entity1->Transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * static_cast<glm::vec2>(Entity1->Transform.Scale), Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = GetAncoredPosition2D(Entity2->Transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * static_cast<glm::vec2>(Entity2->Transform.Scale), Entity2->EntityAnchor);

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

std::vector<std::string> Utils::SeparateString(std::string _string, char _seperator)
{
	std::stringstream ss(_string);
	std::vector<std::string> result;

	while (ss.good())
	{
		std::string substr;
		getline(ss, substr, _seperator);
		result.push_back(substr);
	}
	return result;
}

Vector3 Utils::StringToVec3(std::string _string)
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	_string = _string.substr(3, _string.length() - 3);
	return Vector3(x, y, z);
}

Rotator Utils::StringToRotator(std::string _string)
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	_string = _string.substr(3, _string.length() - 3);
	return Rotator(x, y, z);
}

// TODO: Change sphere center to sphere transform (account for rotation and scale)
bool Utils::CheckSphereHit(Vector3 RayStart, Vector3 RayDirection, Vector3 SphereCenter, float SphereRadius, Vector3& HitPos)
{
	const Vector3 Offset = SphereCenter - RayStart;
	// float a = RayDirection.Dot(RayDirection);
	// float b = 2.0f * V.Dot(RayDirection);
	// float c = V.Dot(V) - EntityCheck->EntityMesh->m_fWidth * EntityCheck->EntityMesh->m_fWidth;
	float a = RayDirection.Dot(RayDirection);// glm::dot(RayDirection, RayDirection); // TODO: This isn't accepted? Doesn't work out type?
	float b = 2.0f * Offset.Dot(RayDirection);
	float c = Offset.Dot(Offset) - SphereRadius * SphereRadius;
	float d = b * b - 4 * a * c;

	if (d > 0)
	{
		float x1 = (-b - sqrt(d)) / 2;
		float x2 = (-b + sqrt(d)) / 2;

		float XPos = RayStart.X + RayDirection.X * -x2;
		float YPos = RayStart.Y + RayDirection.Y * -x2;
		float ZPos = RayStart.Z + RayDirection.Z * -x2;
		HitPos = {XPos, YPos, ZPos};

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
	if (d <= 0)
	{
		//std::cout << "Ray cast missed!\n";
	}
	return false;
}

// TODO: Change transform (account for rotation and scale)
bool Utils::CheckCubeHit(Vector3 RayStart, Vector3 RayDirection, Vector3 CubeDimensions, Pointer<Entity> EntityCheck, Vector3& HitPos)
{
	Vector3 HalfDimensionvec = Vector3(CubeDimensions.X / 2.0f, CubeDimensions.Y / 2.0f, CubeDimensions.Z / 2.0f);
	std::vector<Vector3> HitPositions;
	if (CheckFaceHit(Vector3(-HalfDimensionvec.X, -HalfDimensionvec.Y, HalfDimensionvec.Z), Vector3(HalfDimensionvec.X, HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(Vector3(-HalfDimensionvec.X, -HalfDimensionvec.Y, -HalfDimensionvec.Z), Vector3(HalfDimensionvec.X, HalfDimensionvec.Y, -HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(Vector3(-HalfDimensionvec.X, -HalfDimensionvec.Y, -HalfDimensionvec.Z), Vector3(HalfDimensionvec.X, -HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(Vector3(-HalfDimensionvec.X, HalfDimensionvec.Y, -HalfDimensionvec.Z), Vector3(HalfDimensionvec.X, HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(Vector3(-HalfDimensionvec.X, -HalfDimensionvec.Y, -HalfDimensionvec.Z), Vector3(-HalfDimensionvec.X, HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(Vector3(HalfDimensionvec.X, -HalfDimensionvec.Y, -HalfDimensionvec.Z), Vector3(HalfDimensionvec.X, HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}

	if (HitPositions.size() <= 0)
	{
		return false;
	}
	HitPos = HitPositions[0];

	if (HitPositions.size() == 1)
	{
		return true;
	}

	for (int i = 0; i < HitPositions.size(); i++)
	{
		if (glm::length(RayStart - HitPositions[i]) < glm::length(RayStart - HitPos))
		{
			HitPos = HitPositions[i];
		}
	}
	return true;
}

bool Utils::CheckPlaneEntityHit(Vector3 RayStart, Vector3 RayDirection, Pointer<Entity> EntityCheck, Vector3& HitPos)
{
	glm::vec3 HalfDimensionvec = glm::vec3(EntityCheck->EntityMesh->m_fWidth / 2.0f, EntityCheck->EntityMesh->m_fHeight / 2.0f, EntityCheck->EntityMesh->m_fDepth / 2.0f);
	if (CheckFaceHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		return true;
	}
	return false;
}

Vector3 Utils::LinePlaneIntersect(Vector3 RayStart, Vector3 RayDirection, Vector3 PlanePos, Vector3 PlaneNormal)
{
	Vector3 diff = RayStart - PlanePos;
	double prod1 = diff.Dot(PlaneNormal);
	double prod2 = RayDirection.Dot(PlaneNormal);
	float prod3 = prod1 / prod2;
	return RayStart - RayDirection * prod3;
}

// TODO: Remove need for entity (use transform/matrix)
bool Utils::CheckFaceHit(Vector3 BottomLeftOffset, Vector3 TopRightOffset, Vector3 RayStart, Vector3 RayDirection, Pointer<Entity> EntityCheck, Vector3& HitPos)
{
	Vector3 AnchoredPosition = GetAncoredPosition(EntityCheck->Transform.Position, glm::vec3(EntityCheck->EntityMesh->m_fWidth, EntityCheck->EntityMesh->m_fHeight, EntityCheck->EntityMesh->m_fDepth), EntityCheck->EntityAnchor);
	Vector3 lb = BottomLeftOffset + AnchoredPosition;
	Vector3 rt = TopRightOffset + AnchoredPosition;
	// TODO: Confirm matrix to vector order for new system and create/convert
	lb = Vector3(Vector4(lb, 1.0f) * EntityCheck->GetModel().ToGLM());
	rt = Vector3(Vector4(rt, 1.0f) * EntityCheck->GetModel().ToGLM());

	Vector3 DirFrac = 1.0f / RayDirection;

	float t1 = (lb.X - RayStart.X) * DirFrac.X;
	float t2 = (rt.X - RayStart.X) * DirFrac.X;
	float t3 = (lb.Y - RayStart.Y) * DirFrac.Y;
	float t4 = (rt.Y - RayStart.Y) * DirFrac.Y;
	float t5 = (lb.Z - RayStart.Z) * DirFrac.Z;
	float t6 = (rt.Z - RayStart.Z) * DirFrac.Z;

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
