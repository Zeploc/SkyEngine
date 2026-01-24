// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "MeshComponent.h"

// Engine Includes //
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "Render/Renderer.h"
#include "Render/Materials/InternalMaterial.h"
#include "System/LogManager.h"
#include "System/Utils.h"

CMeshComponent::CMeshComponent(const TSharedPointer<Entity>& InOwner)
	: CComponent(InOwner)
{
	MeshMaterial = GetRenderer()->DefaultMaterial;
	
	SetSerializeVariable(MeshAsset);
	SetSerializeVariable(MeshMaterial);
	SetSerializeVariable(bVisible);
}

CMeshComponent::CMeshComponent(const TSharedPointer<Entity>& InOwner, TAssetObjectPointer<CMesh> InMeshAsset, const TAssetObjectPointer<CMaterialInterface>& InMaterial)
: CComponent(InOwner), MeshAsset(InMeshAsset)
{
	if (InMaterial)
	{
		MeshMaterial = InMaterial;
	}
	else
	{
		MeshMaterial = GetRenderer()->DefaultMaterial;
	}
	
	SetSerializeVariable(MeshAsset);
	SetSerializeVariable(MeshMaterial);
	SetSerializeVariable(bVisible);
}

CMeshComponent::~CMeshComponent()
{
	// TODO: Give id (component system?) and link to parent
	CLogManager::Get()->DisplayMessage("Mesh was destroyed!");	
}

void CMeshComponent::OnAttached()
{
	CComponent::OnAttached();
}

void CMeshComponent::Unload()
{
	CComponent::Unload();	
}

bool CMeshComponent::ShouldRender() const
{
	return IsVisible() && MeshAsset;
}

TAssetObjectPointer<CMaterialInterface> CMeshComponent::GetMaterial() const
{
	return MeshMaterial;
}

void CMeshComponent::SetMaterial(TAssetObjectPointer<CMaterialInterface> NewMaterial)
{
	MeshMaterial = NewMaterial;
}

bool CMeshComponent::SetMeshAsset(TAssetObjectPointer<CMesh> NewMeshAsset)
{
	MeshAsset = NewMeshAsset;
	// GetGraphicsAPI()->CleanupMesh(this);
	// BindMeshData();
	return true;
}

uint32_t CMeshComponent::GetVao() const
{
	return GetMeshAsset()->GetVao();
}

int CMeshComponent::GetIndicesCount() const
{
	return GetMeshAsset()->GetIndicesCount();
}

STransform CMeshComponent::GetRenderTransform() const
{
	return GetOwner()->Transform;
}

bool CMeshComponent::CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos)
{
	// TODO: Check against basic box bounds before considering
	
	// CLogManager::Get()->DisplayMessage("No Check Hit for mesh ray hit check!");

	// TODO: Not working with plane?
	return Utils::CheckMeshHit(GetOwner()->Transform, *GetMeshAsset(), RayStart, RayDirection, HitPos);
}

void CMeshComponent::SetVisible(bool bNewVisible)
{
	bVisible = bNewVisible;
}

void CMeshComponent::Serialize(std::ostream& os)
{
	CComponent::Serialize(os);
}

void CMeshComponent::Deserialize(std::istream& is)
{
	CComponent::Deserialize(is);
}

std::string CMeshComponent::GetComponentClassName()
{
	return GetStaticName();
}

std::string CMeshComponent::GetStaticName()
{
	return "MeshComponent";
}
