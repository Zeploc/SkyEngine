// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "MeshComponent.h"

// Engine Includes //
#include "MeshManager.h"
#include "Core/Application.h"
#include "Entity/CollisionBounds.h"
#include "Entity/Entity.h"
#include "Render/Shaders/Shader.h"
#include "System/LogManager.h"
#include "System/Utils.h"

CMeshComponent::CMeshComponent(const TPointer<Entity>& InOwner)
	: CComponent(InOwner)
{
}

CMeshComponent::CMeshComponent(const TPointer<Entity>& InOwner, std::string InMeshAsset, const TPointer<CMaterialInterface>& InMaterial)
: CComponent(InOwner), MeshAsset(InMeshAsset)
{
	MeshMaterial = InMaterial;
}

CMeshComponent::~CMeshComponent()
{
	//if (MeshCollisionBounds) delete MeshCollisionBounds;
	MeshCollisionBounds = nullptr;
	// TODO: Give id (component system?) and link to parent
	CLogManager::Get()->DisplayMessage("Mesh was destroyed!");	
}

void CMeshComponent::OnAttached()
{
	CComponent::OnAttached();
	BindMeshData();
}

bool CMeshComponent::ShouldRender() const
{
	return IsVisible();
}

TPointer<CMaterialInterface> CMeshComponent::GetMaterial() const
{
	return MeshMaterial;
}

void CMeshComponent::SetMaterial(TPointer<CMaterialInterface> NewMaterial)
{
	MeshMaterial = NewMaterial;
}

bool CMeshComponent::SetMeshAsset(std::string NewMeshAssetName)
{
	if (!GetMeshManager()->HasMesh(NewMeshAssetName))
	{
		return false;
	}
	MeshAsset = NewMeshAssetName;
	Rebind();
	// GetGraphicsAPI()->CleanupMesh(this);
	// BindMeshData();
	return true;
}

uint32_t CMeshComponent::GetVao() const
{
	return vao;
}

int CMeshComponent::GetIndicesCount() const
{
	return IndicesCount;
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
	return Utils::CheckMeshHit(GetOwner()->Transform, GetMeshData(), RayStart, RayDirection, HitPos);
}

void CMeshComponent::BindMeshData()
{
	vao = GetGraphicsAPI()->CreateVertexBuffer(GetMeshData());
	CLogManager::Get()->DisplayMessage(std::format("Created Mesh {} with vao {}", MeshAsset, vao));
	
	// TODO: Should be created and bound elsewhere and just pointed to by this component
	CMeshData MeshData = GetMeshData();
	IndicesCount = MeshData.GetIndicesCount();
	MeshData.BindData(vao);
	CLogManager::Get()->DisplayMessage(std::format("Bound Mesh {} with vao {}", MeshAsset, vao));
}

void CMeshComponent::Rebind()
{
	GetGraphicsAPI()->CleanupMesh(this);
	BindMeshData();
}

void CMeshComponent::SetVisible(bool bNewVisible)
{
	bVisible = bNewVisible;
}

void CMeshComponent::AddCollisionBounds(float fWidth, float fHeight, float fDepth, TPointer<Entity> _EntityRef)
{
	MeshCollisionBounds = std::make_shared<CCollisionBounds>(fWidth, fHeight, fDepth, _EntityRef);
}

void CMeshComponent::AddCollisionBounds(TPointer<CCollisionBounds> NewCollision)
{
	MeshCollisionBounds = NewCollision;
}

CMeshData CMeshComponent::GetMeshData()
{
	return GetMeshManager()->GetMesh(MeshAsset);
}
