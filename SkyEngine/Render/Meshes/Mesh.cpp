// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Mesh.h"

// Engine Includes //
#include "Render/Shader.h"

#include "Camera/CameraManager.h"
#include "Entity/CollisionBounds.h"
#include "Entity/Entity.h"
#include "System/LogManager.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Mesh::Mesh()
{

}

/************************************************************
#--Description--#:  Constructor function with texture
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Mesh::~Mesh()
{
	//if (MeshCollisionBounds) delete MeshCollisionBounds;
	MeshCollisionBounds = nullptr;
	// TODO: Give id (component system?) and link to parent
	LogManager::GetInstance()->DisplayLogMessage("Mesh was destroyed!");	
}

// TODO: Remove the need for entity passed through (use transforms/matrix)
bool Mesh::CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, Pointer<Entity> EntityCheck)
{
	// TODO: Check against basic box bounds before considering
	
	LogManager::GetInstance()->DisplayLogMessage("No Check Hit for mesh ray hit check!");

	return false;
}

void Mesh::Reset()
{
	// Reset all mesh variables
	Rebind();
	// Reset Mesh Collision Bounds
	if (MeshCollisionBounds)
	{
		MeshCollisionBounds->Reset();
	}
}

void Mesh::SetLit(bool _bIsLit)
{
	MeshMaterial.bIsLit = _bIsLit;
}

void Mesh::SetReflection(bool _bReflecting)
{
	MeshMaterial.bReflect = _bReflecting;
	// if (bReflection)
	// {
	// 	program = Shader::Programs["ReflectionProgram"];
	// }
	// else
	// {
	// 	if (bIsLit)
	// 	{
	// 		program = Shader::Programs["LitTextureprogram"];
	// 	}
	// 	else
	// 	{
	// 		program = Shader::Programs["BaseProgram"];
	// 	}
	// }
}

void Mesh::AddCollisionBounds(float fWidth, float fHeight, float fDepth, Pointer<Entity> _EntityRef)
{
	MeshCollisionBounds = std::make_shared<CollisionBounds>(fWidth, fHeight, fDepth, _EntityRef);
}

void Mesh::AddCollisionBounds(Pointer<CollisionBounds> NewCollision)
{
	MeshCollisionBounds = NewCollision;
}
