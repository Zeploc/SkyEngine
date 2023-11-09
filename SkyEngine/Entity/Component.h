// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

class Entity;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API CComponent : public std::enable_shared_from_this<CComponent>
{
public:
	virtual ~CComponent() = default;
	CComponent(const TPointer<Entity>& InOwner);
	virtual void OnAttached();
	virtual void BeginPlay();

	virtual void Update();
	virtual void OnDestroy();

	TPointer<Entity> GetOwner() const { return Owner; }

private:
	TPointer<Entity> Owner;
	// TODO: Parent component?
	TPointer<CComponent> ParentComponent;
};
