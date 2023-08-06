// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Vector3.decl.h"
#include "Math/Rotator.h"

template <typename T>
TVector3<T> TVector3<T>::Cross(const TVector3& V) const
{
	return glm::cross<T>(*this, V);
}

template <typename T>
float TVector3<T>::Dot(const TVector3& V) const
{
	return glm::dot<T>(*this, V);
}

template <typename T>
TVector3<T> TVector3<T>::GetNormalized() const
{
	static_assert(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
	return glm::normalize<T, glm::precision::highp>(*this);
}

template <typename T>
TVector3<T>& TVector3<T>::Normalize()
{
	static_assert(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
	return *this = glm::normalize<T, glm::precision::highp>(*this);
}

template <typename T>
T TVector3<T>::Size() const
{
	return glm::length(*this);
}

template <typename T>
void TVector3<T>::Rotate(T Angle, const TVector3& Axis)
{
	glm::mat4 RotateMatrix = glm::rotate(glm::mat4(1), glm::radians(Angle), Axis);
	*this = glm::vec4(*this, 1.0f) * RotateMatrix;
}

template <typename T>
TVector3<T>& TVector3<T>::operator=(TVector3 const& v)
{
	glm::vec3::operator=(v);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector3<T>& TVector3<T>::operator+=(U scalar)
{
	AddValues(scalar, scalar, scalar);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector3<T>& TVector3<T>::operator+=(TVector3<U> other)
{
	AddValues(other.x, other.y, other.z);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector3<T>& TVector3<T>::operator+=(glm::tvec3<U> other)
{
	AddValues(other.x, other.y, other.z);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector3<T>& TVector3<T>::operator-=(U scalar)
{
	MinusValues(scalar, scalar, scalar);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector3<T>& TVector3<T>::operator-=(TVector3<U> other)
{
	MinusValues(other.x, other.y, other.z);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector3<T>& TVector3<T>::operator-=(glm::tvec3<U> other)
{
	MinusValues(other.x, other.y, other.z);
	return *this;
}

template <typename T>
template <typename U>
void TVector3<T>::MinusValues(U InX, U InY, U InZ)
{
	this->x -= static_cast<T>(InX);
	this->y -= static_cast<T>(InY);
	this->z -= static_cast<T>(InZ);
}

template <typename T>
template <typename U>
void TVector3<T>::AddValues(U InX, U InY, U InZ)
{
	this->x += static_cast<T>(InX);
	this->y += static_cast<T>(InY);
	this->z += static_cast<T>(InZ);
}

template <typename T>
SRotator TVector3<T>::ToRotator() const
{
	// Get Angle on the lateral plane (will be in radians)
	const float YawAngle = atan2(X,Z);
	// TODO: Do pitch correctly
	// Get pitch based on Y (Up)
	const float PitchAngle = Size() * sin(Y);

	SRotator NewRotator;
	NewRotator.Yaw = ToDegree(YawAngle);//Magnitude * cos(Angle));
	NewRotator.Pitch = ToDegree(PitchAngle);//ToDegree(Magnitude * sin(Angle));

	// Default roll to 0 ie Z up
	NewRotator.Roll = 0;

	return NewRotator;
}

template <typename T>
std::string TVector3<T>::ToString() const
{
	// TODO: Update c++
	// std::format
	char Str[1024];
	sprintf_s(Str,"(%.02f, %.02f, %.02f)", X, Y, Z);
	return Str;
}

template <typename T>
void TVector3<T>::Print() const
{
	printf("(%.02f, %.02f, %.02f)", X, Y, Z);
}

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

