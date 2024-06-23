// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Math/MathDefinitions.h"

#include <string>

// struct Rotator;
template <typename T>
struct TVector3;

struct ENGINE_API SRotator
{
	float Yaw;
	float Pitch;
	float Roll;

	SRotator()
	{
		Pitch = Yaw = Roll = 0;
	}

	SRotator(float InYaw, float InPitch, float InRoll)
	{
		Yaw = InYaw;
		Pitch = InPitch;
		Roll = InRoll;
	}

	SRotator(const float* pFloat)
	{
		Yaw = pFloat[0];
		Pitch = pFloat[0];
		Roll = pFloat[0];
	}

	SRotator(float f)
	{
		Pitch = Yaw = Roll = f;
	}

	SRotator(glm::vec3 V)
	{
		Yaw = V.x;
		Pitch = V.y;
		Roll = V.z;
	}
	SRotator(const TVector3<float>& V);

	glm::vec3 ToGLM() const
	{
		return glm::vec3(Yaw, Pitch, Roll);
	}

	SRotator& operator+=(const SRotator& r)
	{
		// TODO: Verify
		Yaw += r.Yaw;
		Pitch += r.Pitch;
		Roll += r.Roll;

		return *this;
	}

	SRotator& operator-()
	{
		// TODO: Verify
		Yaw *= -1;
		Pitch *= -1;
		Roll *= -1;

		return *this;
	}

	SRotator& operator-=(const SRotator& r)
	{
		// TODO: Verify
		Yaw -= r.Yaw;
		Pitch -= r.Pitch;
		Roll -= r.Roll;

		return *this;
	}

	SRotator& operator*=(float f)
	{
		Yaw *= f;
		Pitch *= f;
		Roll *= f;

		return *this;
	}

	// TODO: Verify
	operator const float*() const
	{
		return &(Yaw);
	}

	TVector3<float> ToVector() const;
	
	std::string ToString() const
	{
		// TODO: Update c++
		// std::format
		char Str[1024];
		sprintf_s(Str,"(%.02f, %.02f, %.02f)", Yaw, Pitch, Roll);
		return Str;
	}

	void Print() const
	{
		printf("(%.02f, %.02f, %.02f)", Yaw, Pitch, Roll);
	}
	
	friend std::ostream& operator<<(std::ostream& os, const SRotator& InRotator);
	friend std::istream& operator>>(std::istream& is, SRotator& OutRotator);
};