// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Rotator.h"

#include "Vector.h"

Rotator::Rotator(Vector3 V)
{
	Pitch = V.X;
	Yaw = V.Y;
	Roll = V.Z;
}
