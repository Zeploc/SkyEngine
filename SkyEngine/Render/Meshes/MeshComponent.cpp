// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "MeshComponent.h"

// Engine Includes //
#include "Core/Application.h"
#include "Entity/CollisionBounds.h"
#include "Entity/Entity.h"
#include "Render/Shaders/Shader.h"
#include "System/LogManager.h"

CMeshComponent::CMeshComponent(const TPointer<Entity>& InOwner)
	: CComponent(InOwner)
{
}

CMeshComponent::CMeshComponent(const TPointer<Entity>& InOwner, float InWidth, float InHeight, float InDepth, const TPointer<CMaterialInterface>& InMaterial)
: CComponent(InOwner)
{
	m_fWidth = InWidth;
	m_fHeight = InHeight;
	m_fDepth = InDepth;
	MeshMaterial = InMaterial;
}

/************************************************************
#--Description--#:  Constructor function with texture
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
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

// TODO: Remove the need for entity passed through (use transforms/matrix)
bool CMeshComponent::CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos, TPointer<Entity> EntityCheck)
{
	// TODO: Check against basic box bounds before considering
	
	CLogManager::Get()->DisplayMessage("No Check Hit for mesh ray hit check!");

	return false;
}

void CMeshComponent::Reset()
{
	// Reset all mesh variables
	Rebind();
	// Reset Mesh Collision Bounds
	if (MeshCollisionBounds)
	{
		MeshCollisionBounds->Reset();
	}
}

void CMeshComponent::BindMeshData()
{
	MeshData MeshData = GetMeshData();
	IndicesCount = MeshData.GetIndicesCount();
	vao = GetGraphicsAPI()->CreateVertexBuffer(MeshData);
	MeshData.BindData(vao);
	std::cout << "Created mesh with vao: " << vao << std::endl;
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
