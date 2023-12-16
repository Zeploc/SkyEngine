#include "SEPCH.h"
#include "AssetInterface.h"

std::ostream& operator<<(std::ostream& os, IAssetObjectInterface* InAsset)
{
	InAsset->Serialize(os);
	return os;
}

std::istream& operator>>(std::istream& is, IAssetObjectInterface* InAsset)
{
	InAsset->Deserialize(is);
	return is;
}
