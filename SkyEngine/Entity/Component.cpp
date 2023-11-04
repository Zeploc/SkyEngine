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

void CComponent::Update()
{
}

void CComponent::OnDestroy()
{
}