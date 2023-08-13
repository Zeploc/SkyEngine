// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <format>

#include "Core/Core.h"
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Math/MathDefinitions.h"

struct SRotator;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

template <typename T = float>
struct TVector2 : glm::tvec2<T, glm::highp>
{	
	T& X = glm::tvec2<T>::x;
	T& Y = glm::tvec2<T>::y;

	TVector2(T InX, T InY)
	{
		X = InX;
		Y = InY;
	}
	
	TVector2()
		: glm::tvec2<T>()
	{
		
	}
	
	TVector2(const TVector2& Other)
		: glm::tvec2<T>(Other)
	{
		
	}
	TVector2(const glm::tvec2<T>& Other)
		: glm::tvec2<T>(Other)
	{
		
	}
	TVector2 & operator=(TVector2 const & v)
	{
		glm::tvec2<T>::operator=(v);
		return *this;
	}

	template <class U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	TVector2<T>& operator+=(U scalar);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	TVector2<T>& operator+=(TVector2<U> other);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	TVector2<T>& operator+=(glm::tvec2<U> other);
	
	template <class U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	TVector2<T>& operator-=(U scalar);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<T, U>>>
	TVector2<T>& operator-=(TVector2<U> other);
	
	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	TVector2<T>& operator-=(glm::tvec2<U> other);

	TVector2 Cross(const TVector2& V) const
	{
		return glm::cross<T>(*this, V);
	}

	float Dot(const TVector2& V) const
	{
		return glm::dot<T>(*this, V);
	}

	TVector2 GetNormalized() const
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
		return glm::normalize<T, glm::precision::highp>(*this);
	}

	TVector2& Normalize()
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'normalize' accepts only floating-point inputs");
		return *this = glm::normalize<T, glm::precision::highp>(*this);
	}

	T Size() const
	{
		return glm::length(*this);
	}

	/* Vector rotated by an angle and axis */
	void Rotate(T Angle, const TVector2& Axis)
	{
		glm::mat4 RotateMatrix = glm::rotate(glm::mat4(1), glm::radians(Angle), Axis);
		*this = glm::vec4(*this, 1.0f) * RotateMatrix;
	}
	/* Direction vector to a rotator */
	SRotator ToRotator() const;
	
	std::string ToString() const
	{
		return std::format("({}, {})", X, Y);
	}

	void Print() const
	{
		printf("(%.02f, %.02f)", X, Y);
	}

	// TODO:
	// friend std::ostream& operator<<(std::ostream& os, const Vector2x& InVector);
	// friend std::istream& operator>>(std::istream& is, Vector2x& OutVector);

protected:	
	template <typename  U>
	void MinusValues(U InX, U InY);
	
	template <typename  U>
	void AddValues(U InX, U InY);
};

typedef TVector2<float> SVector2;
typedef TVector2<float> SVector2f;
typedef TVector2<int> SVector2i;
