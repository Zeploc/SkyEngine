#include "SEPCH.h"
#include "BoxComponent.h"

#include "Basic/DefaultMeshes.h"
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "System/LogManager.h"

CBoxComponent::CBoxComponent(const TSharedPointer<Entity>& InOwner, const TAssetObjectPointer<CMaterialInterface>& InMaterial)
	: CComponent(InOwner), BoxMaterial(InMaterial)
{
	MeshAsset = DefaultMesh::GetBox();
}

void CBoxComponent::OnAttached()
{
	CComponent::OnAttached();
}

uint32_t CBoxComponent::GetVao() const
{
	return MeshAsset->GetVao();
}

int CBoxComponent::GetIndicesCount() const
{
	return MeshAsset->GetIndicesCount();
}

std::string CBoxComponent::GetComponentClassName()
{
	return GetStaticName();
}

std::string CBoxComponent::GetStaticName()
{
	return "BoxComponent";
}

bool CBoxComponent::ShouldRender() const
{
	return MeshAsset.IsValid();
}

bool CBoxComponent::IsVisible() const
{
	return true;
}

void CBoxComponent::SetVisible(bool bNewVisible)
{
}

TAssetObjectPointer<CMaterialInterface> CBoxComponent::GetMaterial() const
{
	return BoxMaterial;
}

STransform CBoxComponent::GetRenderTransform() const
{
	return GetOwner()->Transform;
}

void CBoxComponent::Deserialize(std::istream& is)
{
	CComponent::Deserialize(is);
	
	BoxMaterial = GetMaterialManager()->FindMaterial("BoxMaterial");
}
