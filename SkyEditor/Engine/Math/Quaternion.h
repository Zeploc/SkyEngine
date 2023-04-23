// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Vector.h"

struct Quaternion
{
	float X, Y, Z, W;

	Quaternion(float _x, float _y, float _z, float _w);

	void Normalize();

	Quaternion Conjugate();

	Vector3 ToDegrees();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3& v);