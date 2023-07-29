// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Rotator.decl.h"
#include "../Vector.h"

#include <string>
#include <sstream>

inline Rotator::Rotator(const Vector3x<float>& V)
{
	Pitch = V.X;
	Yaw = V.Y;
	Roll = V.Z;
}

inline Vector3 Rotator::ToVector() const
{
	// TODO: Confirm correct
	const Vector3 ForwardVector(-cos(glm::radians(Pitch)) * sin(glm::radians(Yaw)),
	                            sin(glm::radians(Pitch)),
	                            -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)));
	return ForwardVector;
}

inline std::ostream& operator<<(std::ostream& os, const Rotator& InRotator)
{
	// TODO: Improve to not use to string
	os << "(";
	os << std::to_string(InRotator.Pitch) << std::string(" ");
	os << std::to_string(InRotator.Yaw) << std::string(" ");
	os << std::to_string(InRotator.Roll);
	os << ")";
	return os;
}

inline std::istream& operator>>(std::istream& is, Rotator& OutRotator)
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

inline Rotator operator+(const Rotator& l, const Rotator& r)
{
	Rotator Ret(l.Pitch + r.Pitch,
				 l.Yaw + r.Yaw,
				 l.Roll + r.Roll);

	return Ret;
}

inline Rotator operator-(const Rotator& l, const Rotator& r)
{
	Rotator Ret(l.Pitch - r.Pitch,
				 l.Yaw - r.Yaw,
				 l.Roll - r.Roll);

	return Ret;
}

inline Rotator operator*(const Rotator& l, float f)
{
	Rotator Ret(l.Pitch * f,
				 l.Yaw * f,
				 l.Roll * f);

	return Ret;
}

inline Rotator operator/(const Rotator& l, float f)
{
	Rotator Ret(l.Pitch / f,
				 l.Yaw / f,
				 l.Roll / f);

	return Ret;
}

inline Rotator operator/(float f, const Rotator& l)
{
	Rotator Ret(f / l.Pitch,
				 f / l.Yaw,
				 f / l.Roll);

	return Ret;
}