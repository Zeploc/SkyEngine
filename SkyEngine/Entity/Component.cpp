#include "Component.h"

CComponent::CComponent(const TPointer<Entity>& InOwner)
{
	Owner = InOwner;
}
