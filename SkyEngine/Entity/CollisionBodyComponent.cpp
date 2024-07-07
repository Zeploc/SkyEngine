#include "SEPCH.h"
#include "CollisionBodyComponent.h"

CCollisionBodyComponent::CCollisionBodyComponent(const THardPointer<Entity>& InOwner, bool bIsStatic)
: CComponent(InOwner), bStaticBody(bIsStatic)
{
}

void CCollisionBodyComponent::SetPhysicsEnabled(bool bNewPhysicsEnabled)
{
}

void CCollisionBodyComponent::Sleep()
{
}

void CCollisionBodyComponent::Wake()
{
}
