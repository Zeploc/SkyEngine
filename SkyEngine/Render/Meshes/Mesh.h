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

#include "MeshData.h"
#include "Render/Material.h"
#include "System/EnumTypes.h"

class Entity;
class CollisionBounds;

class ENGINE_API Mesh
{
public:
	Mesh();
	Mesh(float InWidth, float InHeight, float InDepth, glm::vec4 InColour);
	Mesh(float InWidth, float InHeight, float InDepth, glm::vec4 InColour, const char* InTextureSource);

	virtual ~Mesh();
	
	virtual bool CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, Pointer<Entity> EntityCheck);

	virtual void Update() {}

	virtual void OnDestroy() {}

	virtual void Reset();

	void BindMeshData();
	virtual void Rebind() {}

	// Will replace if texture exists
	virtual void SetLit(bool _bIsLit);

	void SetReflection(bool _bReflecting);

	void AddCollisionBounds(float fHeight, float fWidth, float fDepth, Pointer<Entity> _EntityRef);

	void AddCollisionBounds(Pointer<CollisionBounds> NewCollision);

	Pointer<CollisionBounds> GetCollisionBounds()
	{
		return MeshCollisionBounds;
	};

	Pointer<Material> MeshMaterial;
	float m_fWidth;
	float m_fHeight;
	float m_fDepth = 0;
	GLuint vao;
	int IndicesCount;
	
protected:
	virtual MeshData GetMeshData()= 0;
	
	Pointer<CollisionBounds> MeshCollisionBounds;
};

