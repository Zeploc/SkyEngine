// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

class Entity;

class ENGINE_API CComponent
{
public:
	CComponent(const TPointer<Entity>& InOwner);
	
	TPointer<Entity> Owner;
	// TODO: Parent component
};
