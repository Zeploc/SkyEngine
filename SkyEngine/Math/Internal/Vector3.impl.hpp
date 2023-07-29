// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Vector3.decl.h"
#include "Math/Rotator.h"

template <typename T>
Vector3x<T> Vector3x<T>::Cross(const Vector3x& V) const
{
	return glm::cross<T>(*this, V);
}

template <typename T>
float Vector3x<T>::Dot(const Vector3x& V) const
{
	return glm::dot<T>(*this, V);
}

template <typename T>
Vector3x<T> Vector3x<T>::GetNormalized() const
{
	static_assert(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
	return glm::normalize<T, glm::precision::highp>(*this);
}

template <typename T>
Vector3x<T>& Vector3x<T>::Normalize()
{
	static_assert(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
	return *this = glm::normalize<T, glm::precision::highp>(*this);
}

template <typename T>
T Vector3x<T>::Size() const
{
	return glm::length(*this);
}

template <typename T>
void Vector3x<T>::Rotate(T Angle, const Vector3x& Axis)
{
	glm::mat4 RotateMatrix = glm::rotate(glm::mat4(1), glm::radians(Angle), Axis);
	glm::vec4 Highp_Vec4 = glm::vec4(*this, 1.0f);
	glm::vec4 Tvec4 = Highp_Vec4 * RotateMatrix;
	*this = Tvec4;
}

template <typename T>
Vector3x<T>& Vector3x<T>::operator=(Vector3x const& v)
{
	glm::vec3::operator=(v);
	return *this;
}

template <typename T>
template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
Vector3x<T>& Vector3x<T>::operator+=(U scalar)
{
	AddValues(scalar, scalar, scalar);
	return *this;
}

template <typename T>
template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
Vector3x<T>& Vector3x<T>::operator+=(Vector3x<U> other)
{
	AddValues(other.x, other.y, other.z);
	return *this;
}

template <typename T>
template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
Vector3x<T>& Vector3x<T>::operator+=(glm::tvec3<U> other)
{
	AddValues(other.x, other.y, other.z);
	return *this;
}

template <typename T>
template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
Vector3x<T>& Vector3x<T>::operator-=(U scalar)
{
	MinusValues(scalar, scalar, scalar);
	return *this;
}

template <typename T>
template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
Vector3x<T>& Vector3x<T>::operator-=(Vector3x<U> other)
{
	MinusValues(other.x, other.y, other.z);
	return *this;
}

template <typename T>
template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
Vector3x<T>& Vector3x<T>::operator-=(glm::tvec3<U> other)
{
	MinusValues(other.x, other.y, other.z);
	return *this;
}

template <typename T>
template <typename U>
void Vector3x<T>::MinusValues(U InX, U InY, U InZ)
{
	this->x -= static_cast<T>(InX);
	this->y -= static_cast<T>(InY);
	this->z -= static_cast<T>(InZ);
}

template <typename T>
template <typename U>
void Vector3x<T>::AddValues(U InX, U InY, U InZ)
{
	this->x += static_cast<T>(InX);
	this->y += static_cast<T>(InY);
	this->z += static_cast<T>(InZ);
}

template <typename T>
Rotator Vector3x<T>::ToRotator() const
{
	// Get Angle on the lateral plane (will be in radians)
	const float YawAngle = atan2(X,Z);
	// TODO: Do pitch correctly
	// Get pitch based on Y (Up)
	const float PitchAngle = Size() * sin(Y);

	Rotator NewRotator;
	NewRotator.Yaw = ToDegree(YawAngle);//Magnitude * cos(Angle));
	NewRotator.Pitch = ToDegree(PitchAngle);//ToDegree(Magnitude * sin(Angle));

	// Default roll to 0 ie Z up
	NewRotator.Roll = 0;

	return NewRotator;
}

template <typename T>
std::string Vector3x<T>::ToString() const
{
	// TODO: Update c++
	// std::format
	char Str[1024];
	sprintf_s(Str,"(%.02f, %.02f, %.02f)", X, Y, Z);
	return Str;
}

template <typename T>
void Vector3x<T>::Print() const
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

