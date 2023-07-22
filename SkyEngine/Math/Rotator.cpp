// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Rotator.h"

#include <glm/detail/func_trigonometric.inl>
#include <string>
#include <sstream>

#include "Vector.h"

Rotator::Rotator(Vector3 V)
{
	Pitch = V.X;
	Yaw = V.Y;
	Roll = V.Z;
}

Vector3 Rotator::ToVector() const
{
	// TODO: Confirm correct
	const Vector3 ForwardVector(-cos(glm::radians(Pitch)) * sin(glm::radians(Yaw)),
	                            sin(glm::radians(Pitch)),
	                            -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)));
	return ForwardVector;
}

std::ostream& operator<<(std::ostream& os, const Rotator& InRotator)
{
	// TODO: Improve to not use to string
	os << "(";
	os << std::to_string(InRotator.Pitch) << std::string(" ");
	os << std::to_string(InRotator.Yaw) << std::string(" ");
	os << std::to_string(InRotator.Roll);
	os << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Rotator& OutRotator)
{
	std::string Empty;
	// TODO: Improve bracket removal
	// std::getline(is, Empty, '(');
	std::string StringX, StringY, StringZ;
	is >> StringX >> StringY >> StringZ;
	// TODO: Improve from substring
	OutRotator.Pitch = std::stof(StringX.substr(1, StringX.length() - 1));
	OutRotator.Yaw = std::stof(StringY.substr(0, StringY.length() - 1));
	OutRotator.Roll = std::stof(StringZ.substr(0, StringZ.length() - 1));
	// std::getline(is, Empty, ')');
	return is;
}
