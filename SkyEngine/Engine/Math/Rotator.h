// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <string>

#include "Engine/Math/MathDefinitions.h"

struct Vector3;

struct Rotator
{
	float Pitch;
	float Yaw;
	float Roll;

	Rotator()
	{
		Pitch = Yaw = Roll = 0;
	}

	Rotator(float InPitch, float InYaw, float InRoll)
	{
		Pitch = InPitch;
		Yaw = InYaw;
		Roll = InRoll;
	}

	Rotator(const float* pFloat)
	{
		Pitch = pFloat[0];
		Yaw = pFloat[0];
		Roll = pFloat[0];
	}

	Rotator(float f)
	{
		Pitch = Yaw = Roll = f;
	}

	Rotator(glm::vec3 V)
	{
		Pitch = V.x;
		Yaw = V.y;
		Roll = V.z;
	}
	Rotator(Vector3 V);

	glm::vec3 ToGLM() const
	{
		return glm::vec3(Pitch, Yaw, Roll);
	}

	Rotator& operator+=(const Rotator& r)
	{
		// TODO: Verify
		Pitch += r.Pitch;
		Yaw += r.Yaw;
		Roll += r.Roll;

		return *this;
	}

	Rotator& operator-()
	{
		// TODO: Verify
		Pitch *= -1;
		Yaw *= -1;
		Roll *= -1;

		return *this;
	}

	Rotator& operator-=(const Rotator& r)
	{
		// TODO: Verify
		Pitch -= r.Pitch;
		Yaw -= r.Yaw;
		Roll -= r.Roll;

		return *this;
	}

	Rotator& operator*=(float f)
	{
		Pitch *= f;
		Yaw *= f;
		Roll *= f;

		return *this;
	}

	// TODO: Verify
	operator const float*() const
	{
		return &(Pitch);
	}

	Vector3 ToVector() const;
	
	std::string ToString() const
	{
		// TODO: Update c++
		// std::format
		char Str[1024];
		sprintf_s(Str,"(%.02f, %.02f, %.02f)", Pitch, Yaw, Roll);
		return Str;
	}

	void Print() const
	{
		printf("(%.02f, %.02f, %.02f)", Pitch, Yaw, Roll);
	}
};

inline Rotator operator+(const Rotator& l, const Rotator& r)
{
	Rotator Ret(l.Pitch + r.Pitch,
				 l.Yaw + r.Yaw,
				 l.Roll + r.Roll);

	return Ret;
}

inline Rotator operator-(const Rotator& l, const Rotator& r)
{
	Rotator Ret(l.Pitch - r.Pitch,
				 l.Yaw - r.Yaw,
				 l.Roll - r.Roll);

	return Ret;
}

inline Rotator operator*(const Rotator& l, float f)
{
	Rotator Ret(l.Pitch * f,
				 l.Yaw * f,
				 l.Roll * f);

	return Ret;
}

inline Rotator operator/(const Rotator& l, float f)
{
	Rotator Ret(l.Pitch / f,
				 l.Yaw / f,
				 l.Roll / f);

	return Ret;
}

inline Rotator operator/(float f, const Rotator& l)
{
	Rotator Ret(f / l.Pitch,
				 f / l.Yaw,
				 f / l.Roll);

	return Ret;
}