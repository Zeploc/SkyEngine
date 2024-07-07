#include "SEPCH.h"
#include "Object.h"

std::shared_ptr<CAssetObject> CAssetObject::shared_from_this()
{
	return std::static_pointer_cast<CAssetObject>(CObject::shared_from_this());
}

// TWeakPointer<CAssetObject> CAssetObject::weak_from_this()
// {
// 	return *CObject::weak_from_this();
// }
