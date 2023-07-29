// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once


#include "Core/Core.h"
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Math/MathDefinitions.h"

struct Rotator;

template <typename T = float>
struct ENGINE_API Vector3x : glm::tvec3<T, glm::highp>
{	
	T& X = glm::tvec3<T>::x;
	T& Y = glm::tvec3<T>::y;
	T& Z = glm::tvec3<T>::z;	
	T& R = glm::tvec3<T>::x;
	T& G = glm::tvec3<T>::y;
	T& B = glm::tvec3<T>::z;	

	// TODO: Inherit constructors
	// using glm::tvec3<T>::glm::tvec3;
	
	Vector3x()
		: glm::tvec3<T>()
	{
		
	}
	
	Vector3x(const Vector3x& Other)
		: glm::tvec3<T>(Other)
	{
		
	}
	Vector3x(const glm::tvec3<T>& Other)
		: glm::tvec3<T>(Other)
	{
		
	}
	Vector3x(const glm::tvec2<T>& V2, T Z)
		: glm::tvec3<T>(V2, Z)
	{
		
	}
	
	Vector3x(T InX, T InY, T InZ)
	{
		X = InX;
		Y = InY;
		Z = InZ;
	}
	
	Vector3x & operator=(Vector3x const & v)
	{
		return *this = v;
	}
	
	Vector3x Cross(const Vector3x& V) const
	{
		return glm::cross<T>(*this, V);
	}

	float Dot(const Vector3x& V) const
	{
		return glm::dot<T>(*this, V);
	}

	Vector3x GetNormalized() const
	{
		return glm::normalize(*this);
	}

	Vector3x& Normalize()
	{
		return *this = glm::normalize(*this);
	}

	T Size() const
	{
		return glm::length(*this);
	}

	/* Vector rotated by an angle and axis */
	void Rotate(T Angle, const Vector3x& Axis)
	{
		glm::mat4 RotateMatrix = glm::rotate(glm::mat4(1), glm::radians(Angle), Axis);
		*this = glm::vec4(*this, 1.0f) * RotateMatrix;
	}
	/* Direction vector to a rotator */
	Rotator ToRotator() const;
	
	std::string ToString() const
	{
		// TODO: Update c++
		// std::format
		char Str[1024];
		sprintf_s(Str,"(%.02f, %.02f, %.02f)", X, Y, Z);
		return Str;
	}

	void Print() const
	{
		printf("(%.02f, %.02f, %.02f)", X, Y, Z);
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Vector3x& InVector)
	{
		// TODO: Improve to not use to string
		os << "(";
		os << std::to_string(InVector.X) << std::string(" ");
		os << std::to_string(InVector.Y) << std::string(" ");
		os << std::to_string(InVector.Z);
		os << ")";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, Vector3x& OutVector)
	{
		std::string Empty;
		// TODO: Improve bracket removal
		// std::getline(is, Empty, '(');
		std::string StringX, StringY, StringZ;
		is >> StringX >> StringY >> StringZ;
		// TODO: Improve from substring
		OutVector.X = std::stof(StringX.substr(1, StringX.length() - 1));
		OutVector.Y = std::stof(StringY.substr(0, StringY.length() - 1));
		OutVector.Z = std::stof(StringZ.substr(0, StringZ.length() - 1));
		// std::getline(is, Empty, ')');
		return is;
	}
};

using Vector3 = Vector3x<float>;
using Vector3i = Vector3x<int>;

#include "Rotator.h"

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

