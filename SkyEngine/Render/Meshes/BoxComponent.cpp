#include "SEPCH.h"
#include "BoxComponent.h"

#include "Basic/DefaultMeshes.h"
#include "Core/Application.h"
#include "Entity/Entity.h"
#include "System/LogManager.h"

CBoxComponent::CBoxComponent(const TPointer<Entity>& InOwner, const TPointer<CMaterialInterface>& InMaterial)
	: CComponent(InOwner), BoxMaterial(InMaterial)
{
	BoxMeshName = "Box";
}

void CBoxComponent::OnAttached()
{
	CComponent::OnAttached();
	if (!GetMeshManager()->HasMesh(BoxMeshName))
	{
		GetMeshManager()->AddMesh(BoxMeshName, DefaultMesh::GetBoxData());
	}
}

uint32_t CBoxComponent::GetVao() const
{
	return GetMeshManager()->GetMesh(BoxMeshName).GetVao();
}

int CBoxComponent::GetIndicesCount() const
{
	return GetMeshManager()->GetMesh(BoxMeshName).GetIndicesCount();
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

void CBoxComponent::Deserialize(std::istream& is)
{
	CComponent::Deserialize(is);
	
	BoxMaterial = GetMaterialManager()->FindMaterial("BoxMaterial");
}
