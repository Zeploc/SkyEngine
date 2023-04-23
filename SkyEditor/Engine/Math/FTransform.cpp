// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "FTransform.h"

#include <string>
#include <sstream>

#include "Engine/System/Utils.h"

std::string FTransform::ToString() const
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
		Rotation = Utils::StringToRotator(Seperated[1]);
		Scale = Utils::StringToVec3(Seperated[2]);
	}
}

std::ostream& operator<<(std::ostream& os, const FTransform& InTransform)
{
	os << "(" << InTransform.Position << " " << InTransform.Rotation << " " << InTransform.Scale << ")";
	return os;
}

std::istream& operator>>(std::istream& is, FTransform& OutTransform)
{
	std::string Empty;
	// TODO: Improve bracket removal
	std::getline(is, Empty, '(');
	is >> OutTransform.Position >> OutTransform.Rotation >> OutTransform.Scale;
	//std::getline(is, Empty, ')');
	return is;
}
