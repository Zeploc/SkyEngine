// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Component.h"

CComponent::CComponent(const TPointer<Entity>& InOwner)
{
	Owner = InOwner;
}
