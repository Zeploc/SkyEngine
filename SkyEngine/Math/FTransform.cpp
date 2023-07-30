// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "FTransform.h"

#include <string>
#include <sstream>

#include "System/Utils.h"

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

glm::mat4 FTransform::GetModelMatrix() const
{
	glm::mat4 translate = glm::translate(glm::mat4(), Position);
	glm::mat4 scale = glm::scale(glm::mat4(), Scale);
	glm::mat4 rotation = glm::mat4();
	rotation = rotate(rotation, glm::radians(Rotation.Yaw), glm::vec3(0, 1, 0));
	rotation = rotate(rotation, glm::radians(Rotation.Pitch), glm::vec3(1, 0, 0));
	rotation = rotate(rotation, glm::radians(Rotation.Roll), glm::vec3(0, 0, 1));

	return translate * rotation * scale;
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
