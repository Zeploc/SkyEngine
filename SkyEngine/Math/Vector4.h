// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Math/MathDefinitions.h"

template <typename T = float>
struct ENGINE_API Vector4x : glm::tvec4<T, glm::highp>
{	
	T& X = glm::tvec4<T, glm::highp>::x;
	T& Y = glm::tvec4<T, glm::highp>::y;
	T& Z = glm::tvec4<T, glm::highp>::z;
	T& W = glm::tvec4<T, glm::highp>::w;
	T& R = glm::tvec4<T, glm::highp>::x;
	T& G = glm::tvec4<T, glm::highp>::y;
	T& B = glm::tvec4<T, glm::highp>::z;
	T& A = glm::tvec4<T, glm::highp>::w;

	Vector4x()
		: glm::tvec4<T>()
	{
		
	}
	
	Vector4x(const Vector4x<T>& Other)
		: glm::tvec4<T>(Other)
	{
		
	}
	Vector4x(const glm::tvec4<T>& Other)
		: glm::tvec4<T>(Other)
	{
		
	}
	Vector4x(T InX, T InY, T InZ, T InW)
	{
		X = InX;
		Y = InY;
		Z = InZ;
		W = InW;
	}
	Vector4x(glm::tvec3<T> Vec3, T InW)
		: glm::tvec4<T>(Vec3, InW)
	{
	}
	
	Vector4x& operator=(Vector4x const & v)
	{
		glm::vec4::operator=(v);
		return *this;
	}

	Vector4x Cross(const Vector4x& V) const
	{
		return glm::cross<T>(*this, V);
	}

	float Dot(const Vector4x& V) const
	{
		return glm::dot<T>(*this, V);
	}

	Vector4x GetNormalized() const
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
		return glm::normalize<T, glm::precision::highp>(*this);
	}

	Vector4x& Normalize()
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
		return *this = glm::normalize<T, glm::precision::highp>(*this);
	}

	T Size() const
	{
		return glm::length(*this);
	}
	
	// TODO: Fix this vector not working with glm::value_ptr()
	const float* ToValuePtr() const
	{
		return &X;
	}
	
	std::string ToString() const
	{
		// TODO: Update c++
		// std::format
		char Str[1024];
		sprintf_s(Str,"(%.02f, %.02f, %.02f, %.02f)", X, Y, Z, W);
		return Str;
	}

	void Print() const
	{
		printf("(%.02f, %.02f, %.02f, %.02f)", X, Y, Z, W);
	}

	// TODO:
	// friend std::ostream& operator<<(std::ostream& os, const Vector4x& InVector);
	// friend std::istream& operator>>(std::istream& is, Vector4x& OutVector);
};

using Vector4 = Vector4x<float>;
using Vector4i = Vector4x<int>;