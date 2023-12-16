// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Math/MathDefinitions.h"

template <typename T = float>
struct TVector4 : glm::tvec4<T, glm::highp>
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

friend std::ostream& operator<<(std::ostream& os, const TVector4<T>& InVector)
	{
		// TODO: Improve to not use to string
		os << "(";
		os << std::to_string(InVector.X) << std::string(" ");
		os << std::to_string(InVector.Y) << std::string(" ");
		os << std::to_string(InVector.Z) << std::string(" ");
		os << std::to_string(InVector.W);
		os << ")";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, TVector4<T>& OutVector)
	{
		std::string Empty;
		// TODO: Improve bracket removal
		// std::getline(is, Empty, '(');
		std::string StringX, StringY, StringZ, StringW;
		is >> StringX >> StringY >> StringZ >> StringW;
		// TODO: Improve from substring
		OutVector.X = std::stof(StringX.substr(1, StringX.length() - 1));
		OutVector.Y = std::stof(StringY.substr(0, StringY.length() - 1));
		OutVector.Z = std::stof(StringZ.substr(0, StringZ.length() - 1));
		OutVector.W = std::stof(StringW.substr(0, StringW.length() - 1));
		// std::getline(is, Empty, ')');
		return is;
	}
};

typedef ENGINE_API TVector4<float> SVector4;
typedef ENGINE_API TVector4<float> SVector4f;
typedef ENGINE_API TVector4<int> SVector4i;