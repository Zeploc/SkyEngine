// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "MeshData.h"
#include "Entity/Component.h"
#include "Render/SceneVisual.h"

class ENGINE_API CBoxComponent : public CComponent, public ISceneVisual
{
public:
	CBoxComponent(const TPointer<Entity>& InOwner, const TPointer<CMaterialInterface>& InMaterial);
	// using CComponent::CComponent;
	void OnAttached() override;	
	
	uint32_t GetVao() const override;
	int GetIndicesCount() const override;
	bool ShouldRenderFaces() const override { return false; }
	
public:
	bool ShouldRender() const override;
	bool IsVisible() const override;
	void SetVisible(bool bNewVisible) override;
	TPointer<CMaterialInterface> GetMaterial() const override;
	STransform GetRenderTransform() const override;
protected:
	TPointer<CMaterialInterface> BoxMaterial;
	std::string BoxMeshName;
};
