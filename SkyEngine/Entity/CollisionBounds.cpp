// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "CollisionBounds.h"

// Engine Includes //
#include "Render/Meshes/MeshComponent.h"
#include "System/Utils.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
CCollisionBounds::CCollisionBounds(float _fWidth, float _fHeight, float _fDepth, THardPointer<Entity> _EntityRef)
: EntityRef(_EntityRef), fHeight(_fHeight), fWidth(_fWidth), fDepth(_fDepth)
{

}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
CCollisionBounds::~CCollisionBounds()
{
}

void CCollisionBounds::Reset()
{

}

void CCollisionBounds::SetOffset(glm::vec3 _NewOffset)
{
	v3Offset = _NewOffset;
}

bool CCollisionBounds::isColliding(THardPointer<Entity> Entity2)
{
	if (!Entity2)
	{
		return false;
	}
	// TODO: 3D Collision
	// THardPointer<CMeshComponent> Entity1Mesh = std::dynamic_pointer_cast<CMeshComponent>(EntityRef->EntityMesh);
	// THardPointer<CMeshComponent> Entity2Mesh = std::dynamic_pointer_cast<CMeshComponent>(Entity2->EntityMesh);
	// float HalfWidth1 = (fWidth) / 2; // * * abs(EntityRef->Transform.Scale.x);
	// float HalfHeight1 = (fHeight) / 2; // * * abs(EntityRef->Transform.Scale.y);
	// float HalfDepth1 = (fDepth) / 2; // * * abs(EntityRef->Transform.Scale.z);
	// float HalfWidth2 = (Entity2Mesh->GetCollisionBounds()->fWidth) / 2; // * * abs(Entity2->Transform.Scale.x);
	// float HalfHeight2 = (Entity2Mesh->GetCollisionBounds()->fHeight) / 2; // * * abs(Entity2->Transform.Scale.y);
	// float HalfDepth2 = (Entity2Mesh->GetCollisionBounds()->fDepth) / 2; // * * abs(Entity2->Transform.Scale.z);
	//
	// glm::vec3 Entity1Pos = EntityRef->Transform.Position;
	// glm::vec3 Entity2Pos = Entity2->Transform.Position;
	//
	// if (Entity1Pos.x + Entity1Mesh->GetCollisionBounds()->v3Offset.x + HalfWidth1 > Entity2Pos.x + Entity2Mesh->GetCollisionBounds()->v3Offset.x - HalfWidth2
	// 	&& Entity1Pos.x + Entity1Mesh->GetCollisionBounds()->v3Offset.x - HalfWidth1 < Entity2Pos.x + Entity2Mesh->GetCollisionBounds()->v3Offset.x + HalfWidth2
	// 	&& Entity1Pos.y + Entity1Mesh->GetCollisionBounds()->v3Offset.y + HalfHeight1 > Entity2Pos.y + Entity2Mesh->GetCollisionBounds()->v3Offset.y - HalfHeight2
	// 	&& Entity1Pos.y + Entity1Mesh->GetCollisionBounds()->v3Offset.y - HalfHeight1 < Entity2Pos.y + Entity2Mesh->GetCollisionBounds()->v3Offset.y + HalfHeight2
	// 	&& Entity1Pos.z + Entity1Mesh->GetCollisionBounds()->v3Offset.z + HalfDepth1 > Entity2Pos.z + Entity2Mesh->GetCollisionBounds()->v3Offset.z - HalfDepth2
	// 	&& Entity1Pos.z + Entity1Mesh->GetCollisionBounds()->v3Offset.z - HalfDepth1 < Entity2Pos.z + Entity2Mesh->GetCollisionBounds()->v3Offset.z + HalfDepth2)
	// {
	// 	return true;
	// }
	return false;
}

bool CCollisionBounds::CheckCollision(THardPointer<Entity> Entity2, glm::vec3 Movement)
{
	// TODO: 3D Collision
	// THardPointer<CMeshComponent> Entity1Mesh = std::dynamic_pointer_cast<CMeshComponent>(EntityRef->EntityMesh);
	// THardPointer<CMeshComponent> Entity2Mesh = std::dynamic_pointer_cast<CMeshComponent>(Entity2->EntityMesh);
	// float HalfWidth1 = (fWidth) / 2; // *abs(EntityRef->Transform.Scale.x);
	// float HalfHeight1 = (fHeight) / 2; // *abs(EntityRef->Transform.Scale.y);
	// float HalfDepth1 = (fDepth) / 2; // *abs(EntityRef->Transform.Scale.z);
	// float HalfWidth2 = (Entity2Mesh->GetCollisionBounds()->fWidth) / 2; // *abs(Entity2->Transform.Scale.x);
	// float HalfHeight2 = (Entity2Mesh->GetCollisionBounds()->fHeight) / 2; // *abs(Entity2->Transform.Scale.y);
	// float HalfDepth2 = (Entity2Mesh->GetCollisionBounds()->fDepth) / 2; // *abs(Entity2->Transform.Scale.z);
	//
	// glm::vec3 Entity1Pos = EntityRef->Transform.Position;
	// glm::vec3 Entity2Pos = Entity2->Transform.Position;
	//
	// if (Entity1Pos.x + Entity1Mesh->GetCollisionBounds()->v3Offset.x + HalfWidth1 + Movement.x > Entity2Pos.x + Entity2Mesh->GetCollisionBounds()->v3Offset.x - HalfWidth2
	// 	&& Entity1Pos.x + Entity1Mesh->GetCollisionBounds()->v3Offset.x - HalfWidth1 + Movement.x < Entity2Pos.x + Entity2Mesh->GetCollisionBounds()->v3Offset.x + HalfWidth2
	// 	&& Entity1Pos.y + Entity1Mesh->GetCollisionBounds()->v3Offset.y + HalfHeight1 + Movement.y > Entity2Pos.y + Entity2Mesh->GetCollisionBounds()->v3Offset.y - HalfHeight2
	// 	&& Entity1Pos.y + Entity1Mesh->GetCollisionBounds()->v3Offset.y - HalfHeight1 + Movement.y < Entity2Pos.y + Entity2Mesh->GetCollisionBounds()->v3Offset.y + HalfHeight2
	// 	&& Entity1Pos.z + Entity1Mesh->GetCollisionBounds()->v3Offset.z + HalfDepth1 + Movement.z > Entity2Pos.z + Entity2Mesh->GetCollisionBounds()->v3Offset.z - HalfDepth2
	// 	&& Entity1Pos.z + Entity1Mesh->GetCollisionBounds()->v3Offset.z - HalfDepth1 + Movement.z < Entity2Pos.z + Entity2Mesh->GetCollisionBounds()->v3Offset.z + HalfDepth2)
	// {
	// 	return true;
	// }
	return false;
}

glm::vec3 CCollisionBounds::GetDistance(THardPointer<Entity> Entity2)
{
	// TODO: 3D Collision
	glm::vec3 fDistance = glm::vec3(0, 0, 0);
	// THardPointer<CMeshComponent> Entity1Mesh = std::dynamic_pointer_cast<CMeshComponent>(EntityRef->EntityMesh);
	// THardPointer<CMeshComponent> Entity2Mesh = std::dynamic_pointer_cast<CMeshComponent>(Entity2->EntityMesh);
	// float HalfWidth1 = (fWidth) / 2; // * * abs(EntityRef->Transform.Scale.x);
	// float HalfHeight1 = (fHeight) / 2; // * * abs(EntityRef->Transform.Scale.y);
	// float HalfDepth1 = (fDepth) / 2; // * * abs(EntityRef->Transform.Scale.z);
	// float HalfWidth2 = (Entity2Mesh->GetCollisionBounds()->fWidth) / 2; // * * abs(Entity2->Transform.Scale.x);
	// float HalfHeight2 = (Entity2Mesh->GetCollisionBounds()->fHeight) / 2; // * * abs(Entity2->Transform.Scale.y);
	// float HalfDepth2 = (Entity2Mesh->GetCollisionBounds()->fDepth) / 2; // * * abs(Entity2->Transform.Scale.z);
	//
	// glm::vec3 Entity1Pos = EntityRef->Transform.Position;
	// glm::vec3 Entity2Pos = Entity2->Transform.Position;
	//
	// if (Entity1Pos.x + Entity1Mesh->GetCollisionBounds()->v3Offset.x + HalfWidth1 <= Entity2Pos.x + Entity2Mesh->GetCollisionBounds()->v3Offset.x - HalfWidth2)
	// {
	// 	fDistance.x = abs((Entity1Pos.x + Entity1Mesh->GetCollisionBounds()->v3Offset.x + HalfWidth1) - (Entity2Pos.x + Entity2Mesh->GetCollisionBounds()->v3Offset.x - HalfWidth2));
	// }
	// else if (Entity1Pos.x + Entity1Mesh->GetCollisionBounds()->v3Offset.x - HalfWidth1 >= Entity2Pos.x + Entity2Mesh->GetCollisionBounds()->v3Offset.x + HalfWidth2)
	// {
	// 	fDistance.x = abs((Entity1Pos.x + Entity1Mesh->GetCollisionBounds()->v3Offset.x - HalfWidth1) - (Entity2Pos.x + Entity2Mesh->GetCollisionBounds()->v3Offset.x + HalfWidth2));
	// }
	//
	// if (Entity1Pos.y + Entity1Mesh->GetCollisionBounds()->v3Offset.y + HalfHeight1 <= Entity2Pos.y + Entity2Mesh->GetCollisionBounds()->v3Offset.y - HalfHeight2)
	// {
	// 	fDistance.y = abs((Entity1Pos.y + Entity1Mesh->GetCollisionBounds()->v3Offset.y + HalfHeight1) - (Entity2Pos.y + Entity2Mesh->GetCollisionBounds()->v3Offset.y - HalfHeight2));
	// }
	// else if (Entity1Pos.y + Entity1Mesh->GetCollisionBounds()->v3Offset.y - HalfHeight1 >= Entity2Pos.y + Entity2Mesh->GetCollisionBounds()->v3Offset.y + HalfHeight2)
	// {
	// 	fDistance.y = abs((Entity1Pos.y + Entity1Mesh->GetCollisionBounds()->v3Offset.y - HalfHeight1) - (Entity2Pos.y + Entity2Mesh->GetCollisionBounds()->v3Offset.y + HalfHeight2));
	// }
	//
	// if (Entity1Pos.z + Entity1Mesh->GetCollisionBounds()->v3Offset.z + HalfDepth1 <= Entity2Pos.z + Entity2Mesh->GetCollisionBounds()->v3Offset.z - HalfDepth2)
	// {
	// 	fDistance.z = abs((Entity1Pos.z + Entity1Mesh->GetCollisionBounds()->v3Offset.z + HalfDepth1) - (Entity2Pos.z + Entity2Mesh->GetCollisionBounds()->v3Offset.z - HalfDepth2));
	// }
	// else if (Entity1Pos.z + Entity1Mesh->GetCollisionBounds()->v3Offset.z - HalfDepth1 >= Entity2Pos.z + Entity2Mesh->GetCollisionBounds()->v3Offset.z + HalfDepth2)
	// {
	// 	fDistance.z = abs((Entity1Pos.z + Entity1Mesh->GetCollisionBounds()->v3Offset.z - HalfDepth1) - (Entity2Pos.z + Entity2Mesh->GetCollisionBounds()->v3Offset.z + HalfDepth2));
	// }
	return fDistance;
}
