// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Vector.h"

#include "Quaternion.h"
#include "Rotator.h"

Vector3 Vector3::Cross(const Vector3& V) const
{
	const float _x = Y * V.Z - Z * V.Y;
	const float _y = Z * V.X - X * V.Z;
	const float _z = X * V.Y - Y * V.X;

	return Vector3(_x, _y, _z);
}

float Vector3::Dot(const Vector3& V) const
{
	return X * V.X + Y * V.Y + Z * V.Z;
}

Vector3 Vector3::GetNormalized() const
{
	Vector3 Normalised(*this);
	Normalised.Normalize();

	return Normalised;
}

Vector3& Vector3::Normalize()
{
	const float Length = sqrtf(X * X + Y * Y + Z * Z);

	X /= Length;
	Y /= Length;
	Z /= Length;

	return *this;
}

float Vector3::Size() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

void Vector3::Rotate(float Angle, const Vector3& Axe)
{
	const float SinHalfAngle = sinf(ToRadian(Angle/2));
	const float CosHalfAngle = cosf(ToRadian(Angle/2));

	const float Rx = Axe.X * SinHalfAngle;
	const float Ry = Axe.Y * SinHalfAngle;
	const float Rz = Axe.Z * SinHalfAngle;
	const float Rw = CosHalfAngle;
	Quaternion RotationQ(Rx, Ry, Rz, Rw);

	Quaternion ConjugateQ = RotationQ.Conjugate();
	//  ConjugateQ.Normalize();
	Quaternion W = RotationQ * (*this) * ConjugateQ;

	X = W.X;
	Y = W.Y;
	Z = W.Z;
}

Rotator Vector3::ToRotator() const
{
	// Get Angle on the lateral plane (will be in radians)
	const float YawAngle = atan2(X,Z);
	// Get pitch based on Y (Up)
	const float PitchAngle = sin(Y);

	Rotator NewRotator;
	NewRotator.Yaw = ToDegree(YawAngle);//Magnitude * cos(Angle));
	NewRotator.Pitch = ToDegree(PitchAngle);//ToDegree(Magnitude * sin(Angle));

	// Default roll to 0 ie Z up
	NewRotator.Roll = 0;

	return NewRotator;
}
