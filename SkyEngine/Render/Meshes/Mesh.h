// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Entity/Component.h"
#include "Core/Core.h"

// OpenGL Library Includes //

// Engine Includes //
#include "Math/Transform.h"

// Library Include //
#include <memory>

#include "MeshData.h"
#include "Render/Materials/Material.h"

class Entity;
class CCollisionBounds;

class ENGINE_API CMeshComponent : public CComponent
{
public:
	// TODO: Remove empty constructor once made redundant 
	CMeshComponent();
	// TODO: Replace width with mesh data
	CMeshComponent(const TPointer<Entity>& InOwner, float InWidth, float InHeight, float InDepth, const TPointer<CMaterial>& = nullptr);

	virtual ~CMeshComponent();
	
	virtual bool CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos, TPointer<Entity> EntityCheck);

	virtual void Update() {}

	virtual void OnDestroy() {}

	virtual void Reset();

	void BindMeshData();
	virtual void Rebind() {}

	void AddCollisionBounds(float fHeight, float fWidth, float fDepth, TPointer<Entity> _EntityRef);

	void AddCollisionBounds(TPointer<CCollisionBounds> NewCollision);

	TPointer<CCollisionBounds> GetCollisionBounds()
	{
		return MeshCollisionBounds;
	};

	TPointer<CMaterial> MeshMaterial;
	float m_fWidth;
	float m_fHeight;
	float m_fDepth = 0;
	GLuint vao;
	int IndicesCount;
	
protected:
	virtual MeshData GetMeshData()= 0;
	
	TPointer<CCollisionBounds> MeshCollisionBounds;
};

