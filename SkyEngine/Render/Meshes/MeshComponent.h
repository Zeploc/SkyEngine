// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Entity/Component.h"
#include "Core/Core.h"

// OpenGL Library Includes //

// Engine Includes //
#include "Math/Transform.h"

// Library Include //

#include "MeshData.h"
#include "Render/SceneVisual.h"

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class Entity;
class CShader;
class CCollisionBounds;
class CMaterialInterface;

class ENGINE_API CMeshComponent : public CComponent, public ISceneVisual
{
public:
	// TODO: Remove empty constructor once made redundant 
	CMeshComponent(const TPointer<Entity>& InOwner);
	// TODO: Replace width with mesh data
	CMeshComponent(const TPointer<Entity>& InOwner, float InWidth, float InHeight, float InDepth, const TPointer<CMaterialInterface>& = nullptr);
	virtual ~CMeshComponent();

	void OnAttached() override;	

	virtual bool IsVisible() const override { return bVisible; }
	virtual void SetVisible(bool bNewVisible) override;
	
	bool ShouldRender() const override;
	virtual STransform GetRenderTransform() const override;
	TPointer<CMaterialInterface> GetMaterial() const override;
	virtual void SetMaterial(TPointer<CMaterialInterface> NewMaterial);
	uint32_t GetVao() const override;
	int GetIndicesCount() const override;
	
	virtual bool CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos, TPointer<Entity> EntityCheck);

	virtual void OnDestroy() {}

	virtual void Reset();

	void BindMeshData();
	virtual void Rebind() {}

	void AddCollisionBounds(float fHeight, float fWidth, float fDepth, TPointer<Entity> _EntityRef);

	void AddCollisionBounds(TPointer<CCollisionBounds> NewCollision);

	TPointer<CCollisionBounds> GetCollisionBounds()
	{
		return MeshCollisionBounds;
	}

	TPointer<CMaterialInterface> MeshMaterial;
	float m_fWidth;
	float m_fHeight;
	float m_fDepth = 0;
	uint32_t vao;
	int IndicesCount;
	
protected:
	virtual MeshData GetMeshData()= 0;
protected:
	TPointer<CCollisionBounds> MeshCollisionBounds;

	bool bVisible = true;
};

