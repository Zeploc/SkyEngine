// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Component.h"

#include "Entity.h"
#include "Render/Meshes/BoxComponent.h"
#include "Render/Meshes/MeshComponent.h"

CComponent::CComponent(const THardPointer<Entity>& InOwner)
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

THardPointer<CComponent> CComponent::MakeComponentFromClassName(const std::string& ClassName, THardPointer<Entity> Owner)
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

std::ostream& operator<<(std::ostream& os, const THardPointer<CComponent>& InComponent)
{
	InComponent->Serialize(os);
	return os;
}

std::istream& operator>>(std::istream& is, THardPointer<CComponent>& InComponent)
{
	InComponent->Deserialize(is);
	return is;
}

void CComponent::Serialize(std::ostream& os)
{
	os << " [" << GetComponentClassName() << "] ";
	for (const SSerializableVariable& SerializableVariable : SerializeVariables)
	{
		os << " " << SerializableVariable.GetSerializedVariable();
	}
}

void CComponent::Deserialize(std::istream& is)
{
	for (SSerializableVariable& SerializableVariable : SerializeVariables)
	{
		SerializableVariable.SetDeserializedVariable(is);
	}
}
