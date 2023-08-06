// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

class Entity;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API CComponent
{
public:
	CComponent(const TPointer<Entity>& InOwner);
	
	TPointer<Entity> Owner;
	// TODO: Parent component
};
