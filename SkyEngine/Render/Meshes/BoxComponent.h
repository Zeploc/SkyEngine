// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Mesh.h"
#include "Entity/Component.h"
#include "Render/SceneVisual.h"

class ENGINE_API CBoxComponent : public CComponent, public ISceneVisual
{
public:
	CBoxComponent(const THardPointer<Entity>& InOwner, const TAssetObjectPointer<CMaterialInterface>& InMaterial);
	// using CComponent::CComponent;
	void OnAttached() override;	
	
	uint32_t GetVao() const override;
	int GetIndicesCount() const override;
	bool ShouldRenderFaces() const override { return false; }
	
	std::string GetComponentClassName() override;
	static std::string GetStaticName();
	
public:
	bool ShouldRender() const override;
	bool IsVisible() const override;
	void SetVisible(bool bNewVisible) override;
	TAssetObjectPointer<CMaterialInterface> GetMaterial() const override;
	STransform GetRenderTransform() const override;
protected:
	void Deserialize(std::istream& is) override;
	TAssetObjectPointer<CMaterialInterface> BoxMaterial;
	TAssetObjectPointer<CMesh> MeshAsset;
};
