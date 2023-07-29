// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once


#include "Core/Core.h"
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Math/MathDefinitions.h"

struct Rotator;

template <typename T = float>
struct ENGINE_API Vector2x : glm::tvec2<T, glm::highp>
{	
	T& X = glm::tvec2<T>::x;
	T& Y = glm::tvec2<T>::y;

	Vector2x(T InX, T InY)
	{
		X = InX;
		Y = InY;
	}
	
	Vector2x()
		: glm::tvec2<T>()
	{
		
	}
	
	Vector2x(const Vector2x& Other)
		: glm::tvec2<T>(Other)
	{
		
	}
	Vector2x(const glm::tvec2<T>& Other)
		: glm::tvec2<T>(Other)
	{
		
	}
	Vector2x & operator=(Vector2x const & v)
	{
		return *this = v;
	}

	Vector2x Cross(const Vector2x& V) const
	{
		return glm::cross<T>(*this, V);
	}

	float Dot(const Vector2x& V) const
	{
		return glm::dot<T>(*this, V);
	}

	Vector2x GetNormalized() const
	{
		return glm::normalize(*this);
	}

	Vector2x& Normalize()
	{
		return *this = glm::normalize(*this);
	}

	T Size() const
	{
		return glm::length(*this);
	}

	/* Vector rotated by an angle and axis */
	void Rotate(T Angle, const Vector2x& Axis)
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
		sprintf_s(Str,"(%.02f, %.02f)", X, Y);
		return Str;
	}

	void Print() const
	{
		printf("(%.02f, %.02f)", X, Y);
	}

	// TODO:
	// friend std::ostream& operator<<(std::ostream& os, const Vector2x& InVector);
	// friend std::istream& operator>>(std::istream& is, Vector2x& OutVector);
};

using Vector2 = Vector2x<float>;
using Vector2i = Vector2x<int>;

#include "Rotator.h"

template <typename T>
Rotator Vector2x<T>::ToRotator() const
{
	return Rotator();
}
