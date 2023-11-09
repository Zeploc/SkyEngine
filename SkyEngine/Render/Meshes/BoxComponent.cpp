#include "SEPCH.h"
#include "BoxComponent.h"

#include "Basic/DefaultMeshes.h"
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "System/LogManager.h"

CBoxComponent::CBoxComponent(const TPointer<Entity>& InOwner, const TPointer<CMaterialInterface>& InMaterial)
	: CComponent(InOwner), BoxMaterial(InMaterial)
{
}

void CBoxComponent::OnAttached()
{
	CComponent::OnAttached();
	BindMeshData();
}

uint32_t CBoxComponent::GetVao() const
{
	return vao;
}

int CBoxComponent::GetIndicesCount() const
{
	return IndicesCount;
}

void CBoxComponent::BindMeshData()
{
	CMeshData MeshData = DefaultMesh::GetBoxData();
	vao = GetGraphicsAPI()->CreateVertexBuffer(MeshData);
	CLogManager::Get()->DisplayMessage(std::format("Created box with vao {}", vao));
	IndicesCount = MeshData.GetIndicesCount();
	MeshData.BindData(vao);
}

bool CBoxComponent::ShouldRender() const
{
	return true;
}

bool CBoxComponent::IsVisible() const
{
	return true;
}

void CBoxComponent::SetVisible(bool bNewVisible)
{
}

TPointer<CMaterialInterface> CBoxComponent::GetMaterial() const
{
	return BoxMaterial;
}

STransform CBoxComponent::GetRenderTransform() const
{
	return GetOwner()->Transform;
}
