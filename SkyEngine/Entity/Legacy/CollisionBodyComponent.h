// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Entity/Component.h"

class CCollisionBodyComponent : public CComponent
{
public:
	CCollisionBodyComponent(const TSharedPointer<Entity>& InOwner, bool bIsStatic);

	virtual void SetPhysicsEnabled(bool bNewPhysicsEnabled);
	virtual bool IsPhysicsEnabled() = 0;

	virtual void Sleep();
	virtual void Wake();

protected:
	bool bStaticBody = true;
};
