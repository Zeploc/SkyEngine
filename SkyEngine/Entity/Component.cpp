// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Component.h"

#include "Entity.h"

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

std::ostream& operator<<(std::ostream& os, const TPointer<CComponent>& InComponent)
{
	return os;
}

std::istream& operator>>(std::istream& is, TPointer<CComponent>& InComponent)
{
	return is;
}
