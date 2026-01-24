#include "SEPCH.h"
#include "AssetObject.h"

std::shared_ptr<CAssetObject> CAssetObject::shared_from_this()
{
	return std::static_pointer_cast<CAssetObject>(std::enable_shared_from_this<CObject>::shared_from_this());
}

void CAssetObject::Serialize(std::ostream& os)
{
	for (const SSerializableVariable& SerializableVariable : SerializeVariables)
	{
		os << " " << SerializableVariable.GetSerializedVariable();
	}
}

void CAssetObject::Deserialize(std::istream& is)
{
	for (SSerializableVariable& SerializableVariable : SerializeVariables)
	{
		SerializableVariable.SetDeserializedVariable(is);
	}
}
// TWeakPointer<CAssetObject> CAssetObject::weak_from_this()
// {
// 	return *CObject::weak_from_this();
// }
