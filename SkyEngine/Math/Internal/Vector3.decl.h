// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include <string>
#include <iostream>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>

// #include "Math/MathDefinitions.h"

struct SRotator;

// template <typename T>
// struct Vector3x;

template <typename T>
struct ENGINE_API TVector3 : glm::tvec3<T, glm::precision::highp>
{	
	T& X = glm::tvec3<T>::x;
	T& Y = glm::tvec3<T>::y;
	T& Z = glm::tvec3<T>::z;	
	T& R = glm::tvec3<T>::x;
	T& G = glm::tvec3<T>::y;
	T& B = glm::tvec3<T>::z;	

	// TODO: Inherit constructors
	// using glm::tvec3<T>::glm::tvec3;
	
	TVector3()
		: glm::tvec3<T>()
	{
	}
	
	TVector3(const TVector3& Other)
		: glm::tvec3<T>(Other)
	{
	}
	TVector3(const glm::tvec3<T>& Other)
		: glm::tvec3<T>(Other)
	{
	}
	TVector3(const glm::tvec2<T>& V2, T Z)
		: glm::tvec3<T>(V2, Z)
	{
	}
	TVector3(const glm::tvec4<T, glm::precision::highp>& Other)
		: glm::tvec3<T>(Other)
	{
	}
	
	TVector3(T InX, T InY, T InZ)
	{
		X = InX;
		Y = InY;
		Z = InZ;
	}
	
	TVector3& operator=(TVector3 const & v);

	template <class U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	TVector3<T>& operator+=(U scalar);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	TVector3<T>& operator+=(TVector3<U> other);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	TVector3<T>& operator+=(glm::tvec3<U> other);
	
	template <class U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	TVector3<T>& operator-=(U scalar);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	TVector3<T>& operator-=(TVector3<U> other);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	TVector3<T>& operator-=(glm::tvec3<U> other);
	
	TVector3 Cross(const TVector3& V) const;

	float Dot(const TVector3& V) const;

	TVector3 GetNormalized() const;

	TVector3& Normalize();

	T Size() const;

	// TODO: Fix this vector not working with glm::value_ptr()
	const float* ToValuePtr() const
	{
		return &X;
	}

	/* Vector rotated by an angle and axis */
	void Rotate(T Angle, const TVector3& Axis);

	/* Direction vector to a rotator */
	SRotator ToRotator() const;
	
	std::string ToString() const;

	void Print() const;

	friend std::ostream& operator<<(std::ostream& os, const TVector3& InVector)
	{
		// TODO: Improve to not use to string
		os << "(";
		os << std::to_string(InVector.X) << std::string(" ");
		os << std::to_string(InVector.Y) << std::string(" ");
		os << std::to_string(InVector.Z);
		os << ")";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, TVector3& OutVector)
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

protected:	
	template <typename  U>
	void MinusValues(U InX, U InY, U InZ);
	
	template <typename  U>
	void AddValues(U InX, U InY, U InZ);
};

using SVector = TVector3<float>;
using SVector3 = TVector3<float>;
using SVector3i = TVector3<int>;

