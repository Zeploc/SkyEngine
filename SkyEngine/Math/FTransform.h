// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Rotator.h"
#include "Vector.h"

#include <string>
#include <sstream>

struct ENGINE_API FTransform
{
	Vector3 Position;
	Rotator Rotation;
	Vector3 Scale;

	std::string ToString() const;
	void FromString(std::string sTransform);
	
	friend std::ostream& operator<<(std::ostream& os, const FTransform& InTransform);
	friend std::istream& operator>>(std::istream& is, FTransform& OutTransform);
};
