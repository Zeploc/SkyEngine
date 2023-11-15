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
	CMeshComponent(const TPointer<Entity>& InOwner);
	CMeshComponent(const TPointer<Entity>& InOwner, std::string InMeshAsset, const TPointer<CMaterialInterface>& InMaterial = nullptr);
	virtual ~CMeshComponent();

	void OnAttached() override;
	virtual void Unload() override;

	virtual bool IsVisible() const override { return bVisible; }
	virtual void SetVisible(bool bNewVisible) override;
	
	bool ShouldRender() const override;
	virtual STransform GetRenderTransform() const override;
	
	std::string GetMeshAsset() const { return MeshAsset; }
	virtual bool SetMeshAsset(std::string NewMeshAssetName);
	
	TPointer<CMaterialInterface> GetMaterial() const override;
	virtual void SetMaterial(TPointer<CMaterialInterface> NewMaterial);
	
	uint32_t GetVao() const override;
	int GetIndicesCount() const override;
	bool ShouldRenderFaces() const override { return true; }	
	
	virtual bool CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos);

	virtual void OnDestroy() {}

	void AddCollisionBounds(float fHeight, float fWidth, float fDepth, TPointer<Entity> _EntityRef);

	void AddCollisionBounds(TPointer<CCollisionBounds> NewCollision);

	TPointer<CCollisionBounds> GetCollisionBounds()
	{
		return MeshCollisionBounds;
	}
	
	std::string GetComponentClassName() override;
	static std::string GetStaticName();

	TPointer<CMaterialInterface> MeshMaterial;
	float LEGACY_Width;
	float LEGACY_Height;
	float LEGACY_Depth = 0;
	
protected:
	virtual CMeshData GetMeshData() const;
	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
	TPointer<CCollisionBounds> MeshCollisionBounds;
	std::string MeshAsset;

	bool bVisible = true;
};

