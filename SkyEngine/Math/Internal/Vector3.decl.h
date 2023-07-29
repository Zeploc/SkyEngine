// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include <string>
#include <iostream>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>

// #include "Math/MathDefinitions.h"

struct Rotator;

// template <typename T>
// struct Vector3x;

template <typename T>
struct ENGINE_API Vector3x : glm::tvec3<T, glm::precision::highp>
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
	Vector3x(const glm::tvec4<T, glm::precision::highp>& Other)
		: glm::tvec3<T>(Other)
	{
	}
	
	Vector3x(T InX, T InY, T InZ)
	{
		X = InX;
		Y = InY;
		Z = InZ;
	}
	
	Vector3x& operator=(Vector3x const & v);

	template <class U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	Vector3x<T>& operator+=(U scalar);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	Vector3x<T>& operator+=(Vector3x<U> other);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	Vector3x<T>& operator+=(glm::tvec3<U> other);
	
	template <class U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	Vector3x<T>& operator-=(U scalar);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	Vector3x<T>& operator-=(Vector3x<U> other);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	Vector3x<T>& operator-=(glm::tvec3<U> other);
	
	Vector3x Cross(const Vector3x& V) const;

	float Dot(const Vector3x& V) const;

	Vector3x GetNormalized() const;

	Vector3x& Normalize();

	T Size() const;

	// TODO: Fix this vector not working with glm::value_ptr()
	const float* ToValuePtr() const
	{
		return &X;
	}

	/* Vector rotated by an angle and axis */
	void Rotate(T Angle, const Vector3x& Axis);

	/* Direction vector to a rotator */
	Rotator ToRotator() const;
	
	std::string ToString() const;

	void Print() const;

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

protected:	
	template <typename  U>
	void MinusValues(U InX, U InY, U InZ);
	
	template <typename  U>
	void AddValues(U InX, U InY, U InZ);
};

using Vector3 = Vector3x<float>;
// using Vector3i = Vector3x<int>;

