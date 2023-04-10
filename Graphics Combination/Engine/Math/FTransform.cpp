// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "FTransform.h"

#include <string>

#include "Engine/System/Utils.h"

std::string FTransform::ToString()
{
	std::string sTransform;
	sTransform += Position.ToString() + " ";
	sTransform += Rotation.ToString() + " ";
	sTransform += Scale.ToString();
	return sTransform;
}

void FTransform::FromString(std::string sTransform)
{
	std::vector<std::string> Seperated = Utils::SeparateString(sTransform, ' ');
	if (Seperated.size() >= 3)
	{
		Position = Utils::StringToVec3(Seperated[0]);
		Rotation = Utils::StringToVec3(Seperated[1]);
		Scale = Utils::StringToVec3(Seperated[2]);
	}
}