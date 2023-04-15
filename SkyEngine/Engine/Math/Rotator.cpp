// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Rotator.h"

#include <glm/detail/func_trigonometric.inl>

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
