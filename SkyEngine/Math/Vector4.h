// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Math/MathDefinitions.h"

template <typename T = float>
struct ENGINE_API TVector4 : glm::tvec4<T, glm::highp>
{	
	T& X = glm::tvec4<T, glm::highp>::x;
	T& Y = glm::tvec4<T, glm::highp>::y;
	T& Z = glm::tvec4<T, glm::highp>::z;
	T& W = glm::tvec4<T, glm::highp>::w;
	T& R = glm::tvec4<T, glm::highp>::x;
	T& G = glm::tvec4<T, glm::highp>::y;
	T& B = glm::tvec4<T, glm::highp>::z;
	T& A = glm::tvec4<T, glm::highp>::w;

	TVector4()
		: glm::tvec4<T>()
	{
		
	}
	
	TVector4(const TVector4<T>& Other)
		: glm::tvec4<T>(Other)
	{
		
	}
	TVector4(const glm::tvec4<T>& Other)
		: glm::tvec4<T>(Other)
	{
		
	}
	TVector4(T InX, T InY, T InZ, T InW)
	{
		X = InX;
		Y = InY;
		Z = InZ;
		W = InW;
	}
	TVector4(T InX)
	{
		X = Y = Z = W = InX;
	}
	TVector4(glm::tvec3<T> Vec3, T InW)
		: glm::tvec4<T>(Vec3, InW)
	{
	}
	
	TVector4& operator=(TVector4 const & v)
	{
		glm::vec4::operator=(v);
		return *this;
	}

	TVector4 Cross(const TVector4& V) const
	{
		return glm::cross<T>(*this, V);
	}

	float Dot(const TVector4& V) const
	{
		return glm::dot<T>(*this, V);
	}

	TVector4 GetNormalized() const
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
		return glm::normalize<T, glm::precision::highp>(*this);
	}

	TVector4& Normalize()
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

using SVector4 = TVector4<float>;
using SVector4f = TVector4<float>;
using SVector4i = TVector4<int>;