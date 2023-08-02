// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Quaternion.h"

#include "MathDefinitions.h"
#include "Vector.h"

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	X = _x;
	Y = _y;
	Z = _z;
	W = _w;
}

void Quaternion::Normalize()
{
	float Length = sqrtf(X * X + Y * Y + Z * Z + W * W);

	X /= Length;
	Y /= Length;
	Z /= Length;
	W /= Length;
}

Quaternion Quaternion::Conjugate()
{
	Quaternion ret(-X, -Y, -Z, W);
	return ret;
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
	const float w = (l.W * r.W) - (l.X * r.X) - (l.Y * r.Y) - (l.Z * r.Z);
	const float x = (l.X * r.W) + (l.W * r.X) + (l.Y * r.Z) - (l.Z * r.Y);
	const float y = (l.Y * r.W) + (l.W * r.Y) + (l.Z * r.X) - (l.X * r.Z);
	const float z = (l.Z * r.W) + (l.W * r.Z) + (l.X * r.Y) - (l.Y * r.X);

	Quaternion ret(x, y, z, w);

	return ret;
}

Quaternion operator*(const Quaternion& q, const SVector& v)
{
	const float w = - (q.X * v.X) - (q.Y * v.Y) - (q.Z * v.Z);
	const float x = (q.W * v.X) + (q.Y * v.Z) - (q.Z * v.Y);
	const float y = (q.W * v.Y) + (q.Z * v.X) - (q.X * v.Z);
	const float z = (q.W * v.Z) + (q.X * v.Y) - (q.Y * v.X);

	Quaternion ret(x, y, z, w);

	return ret;
}

SVector Quaternion::ToDegrees()
{
	float f[3];

	f[0] = atan2(X * Z + Y * W, X * W - Y * Z);
	f[1] = acos(-X * X - Y * Y - Z * Z - W * W);
	f[2] = atan2(X * Z - Y * W, X * W + Y * Z);

	f[0] = ToDegree(f[0]);
	f[1] = ToDegree(f[1]);
	f[2] = ToDegree(f[2]);

	return SVector(f[0], f[1], f[2]);
}