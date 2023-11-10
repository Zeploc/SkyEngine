// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Utils.h"

// Library Includes //
#include <sstream>
#include <vector>

// Engine Includes //
#include "LogManager.h"
#include "Entity/Body2DComponent.h"
#include "Entity/Entity.h"
#include "Math/Vector4.h"
#include "Render/Meshes/Basic/Cube.h"
#include "Render/Meshes/Basic/Plane.h"

// Local Includes //

// Static Variables //
int Utils::iEntityNumber = 0;
TPointer<Entity> Utils::WorldCubeMap;

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
glm::vec3 Utils::GetAncoredPosition(SVector position, SVector Dimensions, EANCHOR _AnchorType)
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
bool Utils::isColliding2D(TPointer<Entity> Entity1, TPointer<Entity> Entity2)
{
	TPointer<CCollider2DComponent> Collider1 = Entity1->FindComponent<CCollider2DComponent>();
	TPointer<CCollider2DComponent> Collider2 = Entity2->FindComponent<CCollider2DComponent>();
	if (!Collider1 || !Collider2)
	{
		CLogManager::Get()->DisplayMessage("Missing collider in entity for 2d collide test");
		return false;
	}
	const SBodyShape& Body1 = Collider1->GetBodyShape();
	const SBodyShape& Body2 = Collider2->GetBodyShape();
	if (Body1.BodyType == EBodyType::Circle || Body2.BodyType == EBodyType::Circle)
	{
		CLogManager::Get()->DisplayMessage("Circle colliders not supported yet");
		return false;
	}

	float HalfWidth1 = (Body1.Box.BoxExtents.X) * abs(Entity1->Transform.Scale.X);
	float HalfHeight1 = (Body1.Box.BoxExtents.Y) * abs(Entity1->Transform.Scale.Y);
	float HalfWidth2 = (Body2.Box.BoxExtents.X) * abs(Entity2->Transform.Scale.X);
	float HalfHeight2 = (Body2.Box.BoxExtents.Y) * abs(Entity2->Transform.Scale.Y);

	glm::vec3 Entity1Pos = Entity1->Transform.Position;
	glm::vec3 Entity2Pos = Entity2->Transform.Position;

	if (Entity1Pos.x + HalfWidth1 > Entity2Pos.x - HalfWidth2
		&& Entity1Pos.x - HalfWidth1 < Entity2Pos.x + HalfWidth2
		&& Entity1Pos.y + HalfHeight1 > Entity2Pos.y - HalfHeight2
		&& Entity1Pos.y - HalfHeight1 < Entity2Pos.y + HalfHeight2)
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
bool Utils::CheckCollision2D(TPointer<Entity> Entity1, TPointer<Entity> Entity2, glm::vec2 Movement)
{
	TPointer<CCollider2DComponent> Collider1 = Entity1->FindComponent<CCollider2DComponent>();
	TPointer<CCollider2DComponent> Collider2 = Entity2->FindComponent<CCollider2DComponent>();
	if (!Collider1 || !Collider2)
	{
		CLogManager::Get()->DisplayMessage("Missing collider in entity for 2d collide test");
		return false;
	}
	const SBodyShape& Body1 = Collider1->GetBodyShape();
	const SBodyShape& Body2 = Collider2->GetBodyShape();
	if (Body1.BodyType == EBodyType::Circle || Body2.BodyType == EBodyType::Circle)
	{
		CLogManager::Get()->DisplayMessage("Circle colliders not supported yet");
		return false;
	}

	float HalfWidth1 = (Body1.Box.BoxExtents.X) * abs(Entity1->Transform.Scale.X);
	float HalfHeight1 = (Body1.Box.BoxExtents.Y) * abs(Entity1->Transform.Scale.Y);
	float HalfWidth2 = (Body2.Box.BoxExtents.X) * abs(Entity2->Transform.Scale.X);
	float HalfHeight2 = (Body2.Box.BoxExtents.Y) * abs(Entity2->Transform.Scale.Y);

	glm::vec3 Entity1Pos = Entity1->Transform.Position;
	glm::vec3 Entity2Pos = Entity2->Transform.Position;
	
	if (Entity1Pos.x + HalfWidth1 + Movement.x > Entity2Pos.x - HalfWidth2
		&& Entity1Pos.x - HalfWidth1 + Movement.x < Entity2Pos.x + HalfWidth2
		&& Entity1Pos.y + HalfHeight1 + Movement.y > Entity2Pos.y - HalfHeight2
		&& Entity1Pos.y - HalfHeight1 + Movement.y < Entity2Pos.y + HalfHeight2)
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
glm::vec2 Utils::GetDistance2D(TPointer<Entity> Entity1, TPointer<Entity> Entity2)
{
	glm::vec2 fDistance = glm::vec2(0, 0);
	TPointer<CCollider2DComponent> Collider1 = Entity1->FindComponent<CCollider2DComponent>();
	TPointer<CCollider2DComponent> Collider2 = Entity2->FindComponent<CCollider2DComponent>();
	if (!Collider1 || !Collider2)
	{
		CLogManager::Get()->DisplayMessage("Missing collider in entity for 2d collide test");
		return fDistance;
	}
	const SBodyShape& Body1 = Collider1->GetBodyShape();
	const SBodyShape& Body2 = Collider2->GetBodyShape();
	if (Body1.BodyType == EBodyType::Circle || Body2.BodyType == EBodyType::Circle)
	{
		CLogManager::Get()->DisplayMessage("Circle colliders not supported yet");
		return fDistance;
	}

	float HalfWidth1 = (Body1.Box.BoxExtents.X) * abs(Entity1->Transform.Scale.X);
	float HalfHeight1 = (Body1.Box.BoxExtents.Y) * abs(Entity1->Transform.Scale.Y);
	float HalfWidth2 = (Body2.Box.BoxExtents.X) * abs(Entity2->Transform.Scale.X);
	float HalfHeight2 = (Body2.Box.BoxExtents.Y) * abs(Entity2->Transform.Scale.Y);

	glm::vec3 Entity1Pos = Entity1->Transform.Position;
	glm::vec3 Entity2Pos = Entity2->Transform.Position;

	if (Entity1Pos.x + HalfWidth1 <= Entity2Pos.x - HalfWidth2)
	{
		fDistance.x = abs((Entity1Pos.x + HalfWidth1) - (Entity2Pos.x - HalfWidth2));
	}
	else if (Entity1Pos.x - HalfWidth1 >= Entity2Pos.x + HalfWidth2)
	{
		fDistance.x = abs((Entity1Pos.x - HalfWidth1) - (Entity2Pos.x + HalfWidth2));
	}

	if (Entity1Pos.y + HalfHeight1 <= Entity2Pos.y - HalfHeight2)
	{
		fDistance.y = abs((Entity1Pos.y + HalfHeight1) - (Entity2Pos.y - HalfHeight2));
	}
	else if (Entity1Pos.y - HalfHeight1 >= Entity2Pos.y + HalfHeight2)
	{
		fDistance.y = abs((Entity1Pos.y - HalfHeight1) - (Entity2Pos.y + HalfHeight2));
	}
	return fDistance;
}

/************************************************************
#--Description--#:  Gets the distance with direction from the edges of the entities collision boxes
#--Author--#: 		Alex Coultas
#--Parameters--#:	two pointers to the entities
#--Return--#: 		Returns vector 2 of distance with direction
************************************************************/
glm::vec2 Utils::GetDifference2D(TPointer<Entity> Entity1, TPointer<Entity> Entity2)
{
	glm::vec2 fDistance = glm::vec2(0, 0);
	TPointer<CCollider2DComponent> Collider1 = Entity1->FindComponent<CCollider2DComponent>();
	TPointer<CCollider2DComponent> Collider2 = Entity2->FindComponent<CCollider2DComponent>();
	if (!Collider1 || !Collider2)
	{
		CLogManager::Get()->DisplayMessage("Missing collider in entity for 2d collide test");
		return fDistance;
	}
	const SBodyShape& Body1 = Collider1->GetBodyShape();
	const SBodyShape& Body2 = Collider2->GetBodyShape();
	if (Body1.BodyType == EBodyType::Circle || Body2.BodyType == EBodyType::Circle)
	{
		CLogManager::Get()->DisplayMessage("Circle colliders not supported yet");
		return fDistance;
	}

	float HalfWidth1 = (Body1.Box.BoxExtents.X) * abs(Entity1->Transform.Scale.X);
	float HalfHeight1 = (Body1.Box.BoxExtents.Y) * abs(Entity1->Transform.Scale.Y);
	float HalfWidth2 = (Body2.Box.BoxExtents.X) * abs(Entity2->Transform.Scale.X);
	float HalfHeight2 = (Body2.Box.BoxExtents.Y) * abs(Entity2->Transform.Scale.Y);

	glm::vec3 Entity1Pos = Entity1->Transform.Position;
	glm::vec3 Entity2Pos = Entity2->Transform.Position;

	if (Entity1Pos.x  + HalfWidth1 < Entity2Pos.x - HalfWidth2)
	{
		fDistance.x = (Entity1Pos.x  + HalfWidth1) - (Entity2Pos.x - HalfWidth2);
	}
	else if (Entity1Pos.x  - HalfWidth1 > Entity2Pos.x + HalfWidth2)
	{
		fDistance.x = (Entity1Pos.x  - HalfWidth1) - (Entity2Pos.x + HalfWidth2);
	}

	if (Entity1Pos.y  + HalfHeight1 < Entity2Pos.y  - HalfHeight2)
	{
		fDistance.y = (Entity1Pos.y  + HalfHeight1) - (Entity2Pos.y +  - HalfHeight2);
	}
	else if (Entity1Pos.y  - HalfHeight1 > Entity2Pos.y + HalfHeight2)
	{
		fDistance.y = (Entity1Pos.y  - HalfHeight1) - (Entity2Pos.y + HalfHeight2);
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

SVector Utils::StringToVec3(std::string _string)
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	_string = _string.substr(3, _string.length() - 3);
	return SVector(x, y, z);
}

SRotator Utils::StringToRotator(std::string _string)
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	_string = _string.substr(3, _string.length() - 3);
	return SRotator(x, y, z);
}

bool Utils::CheckMeshHit(STransform MeshTransform, const CMeshData& MeshData, SVector RayStart, SVector RayDirection, SVector& HitPos)
{
	TArray<STriangle> Triangles = MeshData.GetTriangles();
	for (STriangle Triangle : Triangles)
	{
		Triangle.TransformTriangle(MeshTransform);
		if (Triangle.TestHit(RayStart, RayDirection, HitPos))
		{
			return true;
		}
	}
	return false;
}

// TODO: Change sphere center to sphere transform (account for rotation and scale)
bool Utils::CheckSphereHit(SVector RayStart, SVector RayDirection, SVector SphereCenter, float SphereRadius, SVector& HitPos)
{
	const SVector Offset = SphereCenter - RayStart;
	// float a = RayDirection.Dot(RayDirection);
	// float b = 2.0f * V.Dot(RayDirection);
	// float c = V.Dot(V) - EntityCheck->EntityMesh->LEGACY_Width * EntityCheck->EntityMesh->LEGACY_Width;
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
bool Utils::CheckCubeHit(SVector RayStart, SVector RayDirection, SVector CubeDimensions, TPointer<Entity> EntityCheck, SVector& HitPos)
{
	SVector HalfDimensionvec = SVector(CubeDimensions.X / 2.0f, CubeDimensions.Y / 2.0f, CubeDimensions.Z / 2.0f);
	std::vector<SVector> HitPositions;
	if (CheckFaceHit(SVector(-HalfDimensionvec.X, -HalfDimensionvec.Y, HalfDimensionvec.Z), SVector(HalfDimensionvec.X, HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(SVector(-HalfDimensionvec.X, -HalfDimensionvec.Y, -HalfDimensionvec.Z), SVector(HalfDimensionvec.X, HalfDimensionvec.Y, -HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(SVector(-HalfDimensionvec.X, -HalfDimensionvec.Y, -HalfDimensionvec.Z), SVector(HalfDimensionvec.X, -HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(SVector(-HalfDimensionvec.X, HalfDimensionvec.Y, -HalfDimensionvec.Z), SVector(HalfDimensionvec.X, HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(SVector(-HalfDimensionvec.X, -HalfDimensionvec.Y, -HalfDimensionvec.Z), SVector(-HalfDimensionvec.X, HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
	{
		HitPositions.push_back(HitPos);
	}
	if (CheckFaceHit(SVector(HalfDimensionvec.X, -HalfDimensionvec.Y, -HalfDimensionvec.Z), SVector(HalfDimensionvec.X, HalfDimensionvec.Y, HalfDimensionvec.Z), RayStart, RayDirection, EntityCheck, HitPos))
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

bool Utils::CheckPlaneEntityHit(SVector RayStart, SVector RayDirection, TPointer<Entity> EntityCheck, SVector& HitPos)
{
	// Should be component based
	// glm::vec3 HalfDimensionvec = glm::vec3(EntityCheck->EntityMesh->LEGACY_Width / 2.0f, EntityCheck->EntityMesh->LEGACY_Height / 2.0f, EntityCheck->EntityMesh->LEGACY_Depth / 2.0f);
	// if (CheckFaceHit(glm::vec3(-HalfDimensionvec.x, -HalfDimensionvec.y, HalfDimensionvec.z), glm::vec3(HalfDimensionvec.x, HalfDimensionvec.y, HalfDimensionvec.z), RayStart, RayDirection, EntityCheck, HitPos))
	// {
	// 	return true;
	// }
	return false;
}

SVector Utils::LinePlaneIntersect(SVector RayStart, SVector RayDirection, SVector PlanePos, SVector PlaneNormal)
{
	SVector diff = RayStart - PlanePos;
	double prod1 = diff.Dot(PlaneNormal);
	double prod2 = RayDirection.Dot(PlaneNormal);
	float prod3 = float(prod1 / prod2);
	return RayStart - RayDirection * prod3;
}

// TODO: Remove need for entity (use transform/matrix)
bool Utils::CheckFaceHit(SVector BottomLeftOffset, SVector TopRightOffset, SVector RayStart, SVector RayDirection, TPointer<Entity> EntityCheck, SVector& HitPos)
{
	SVector AnchoredPosition = EntityCheck->Transform.Position;
	SVector lb = BottomLeftOffset + AnchoredPosition;
	SVector rt = TopRightOffset + AnchoredPosition;
	// TODO: Confirm matrix to vector order for new system and create/convert
	lb = SVector(SVector4(lb, 1.0f) * EntityCheck->GetModel().ToGLM());
	rt = SVector(SVector4(rt, 1.0f) * EntityCheck->GetModel().ToGLM());

	SVector DirFrac = 1.0f / RayDirection;

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
