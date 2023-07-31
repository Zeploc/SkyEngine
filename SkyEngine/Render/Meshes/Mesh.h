// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// OpenGL Library Includes //
#include <glew/glew.h>
#include <glm/glm.hpp>

// Engine Includes //
#include "Math/FTransform.h"

// Library Include //
#include <memory>

#include "Render/Material.h"
#include "System/EnumTypes.h"

class Entity;
class CollisionBounds;

class ENGINE_API Mesh
{
public:
	Mesh();

	~Mesh();
	
	virtual bool CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, Pointer<Entity> EntityCheck);

	virtual void Update() {}

	virtual void OnDestroy() {}

	virtual void Reset();

	virtual void Rebind() {}

	// Will replace if texture exists
	virtual void SetLit(bool _bIsLit);

	bool IsLit() { return MeshMaterial.bIsLit; }

	void SetReflection(bool _bReflecting);

	bool IsReflecting() { return MeshMaterial.bReflect; }

	void AddCollisionBounds(float fHeight, float fWidth, float fDepth, Pointer<Entity> _EntityRef);

	void AddCollisionBounds(Pointer<CollisionBounds> NewCollision);

	Pointer<CollisionBounds> GetCollisionBounds()
	{
		return MeshCollisionBounds;
	};

	Material MeshMaterial;
	EMESHTYPE m_eShape;
	float m_fWidth;
	float m_fHeight;
	float m_fDepth = 0;
	GLuint vao;
	glm::vec4 UVCoords;
	int m_iIndicies;
protected:
	Pointer<CollisionBounds> MeshCollisionBounds;
};

