// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Vector.h"

#include <iostream>

#include "Quaternion.h"
#include "Rotator.h"

#include <string>
#include <sstream>


// void Vector3::Rotate(float Angle, const Vector3& Axe)
// {
// 	const float SinHalfAngle = sinf(ToRadian(Angle/2));
// 	const float CosHalfAngle = cosf(ToRadian(Angle/2));
//
// 	const float Rx = Axe.X * SinHalfAngle;
// 	const float Ry = Axe.Y * SinHalfAngle;
// 	const float Rz = Axe.Z * SinHalfAngle;
// 	const float Rw = CosHalfAngle;
// 	Quaternion RotationQ(Rx, Ry, Rz, Rw);
//
// 	Quaternion ConjugateQ = RotationQ.Conjugate();
// 	//  ConjugateQ.Normalize();
// 	Quaternion W = RotationQ * (*this) * ConjugateQ;
//
// 	X = W.X;
// 	Y = W.Y;
// 	Z = W.Z;
// }

// Rotator Vector3::ToRotator() const
// {
// 	// Get Angle on the lateral plane (will be in radians)
// 	const float YawAngle = atan2(X,Z);
// 	// TODO: Do pitch correctly
// 	// Get pitch based on Y (Up)
// 	const float PitchAngle = Size() * sin(Y);
//
// 	Rotator NewRotator;
// 	NewRotator.Yaw = ToDegree(YawAngle);//Magnitude * cos(Angle));
// 	NewRotator.Pitch = ToDegree(PitchAngle);//ToDegree(Magnitude * sin(Angle));
//
// 	// Default roll to 0 ie Z up
// 	NewRotator.Roll = 0;
//
// 	return NewRotator;
// }

