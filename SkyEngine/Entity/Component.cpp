// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Component.h"

#include "Entity.h"
#include "Render/Meshes/BoxComponent.h"
#include "Render/Meshes/MeshComponent.h"

CComponent::CComponent(const TPointer<Entity>& InOwner)
{
	Owner = InOwner;
}

void CComponent::OnAttached()
{
}

void CComponent::BeginPlay()
{
}

void CComponent::Unload()
{
}

void CComponent::Update()
{
}

void CComponent::OnDestroy()
{
}

std::string CComponent::GetComponentClassName()
{
	return "Component";
}

TPointer<CComponent> CComponent::MakeComponentFromClassName(const std::string& ClassName, TPointer<Entity> Owner)
{
	if (ClassName == CMeshComponent::GetStaticName())
	{
		return std::make_shared<CMeshComponent>(Owner);
	}
	if (ClassName == CBoxComponent::GetStaticName())
	{
		return std::make_shared<CBoxComponent>(Owner, nullptr);
	}
	return nullptr;
}

std::ostream& operator<<(std::ostream& os, const TPointer<CComponent>& InComponent)
{
	InComponent->Serialize(os);
	return os;
}

std::istream& operator>>(std::istream& is, TPointer<CComponent>& InComponent)
{
	InComponent->Deserialize(is);
	return is;
}

void CComponent::Serialize(std::ostream& os)
{
	os << " [" << GetComponentClassName() << "] ";
}

void CComponent::Deserialize(std::istream& is)
{
}
