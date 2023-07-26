// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once


#include "Core/Core.h"
#include <string>

#include "Math/MathDefinitions.h"

struct Rotator;

struct Vector2i
{
	int x;
	int y;
};

// TODO: Change to vector base
// TODO: Template for type (float, int, double)
// TODO: Use for loops for basic operations in base

struct ENGINE_API Vector2
{
	float X;
	float Y;

	Vector2()
	{
		X = Y = 0.0f;
	}

	Vector2(float _x, float _y)
	{
		X = _x;
		Y = _y;
	}
	Vector2(glm::vec2 V)
	{
		X = V.x;
		Y = V.y;
	}
	glm::vec2 ToGLM() const
	{
		return glm::vec2(X, Y);
	}
	
	std::string ToString() const
	{
		// TODO: Update c++
		// std::format
		char Str[1024];
		sprintf_s(Str,"(%.02f, %.02f)", X, Y);
		return Str;
	}
	
	Vector2& operator+=(const Vector2& r)
	{
		X += r.X;
		Y += r.Y;

		return *this;
	}

	Vector2& operator-()
	{
		X *= -1;
		Y *= -1;

		return *this;
	}

	Vector2& operator-=(const Vector2& r)
	{
		X -= r.X;
		Y -= r.Y;

		return *this;
	}

	Vector2& operator*=(float f)
	{
		X *= f;
		Y *= f;

		return *this;
	}
	
	Vector2 Cross(const Vector2& V) const;

	float Dot(const Vector2& V) const;

	Vector2 GetNormalized();
	Vector2& Normalize();

	float Size() const;
};


inline Vector2 operator+(const Vector2& l, const Vector2& r)
{
	Vector2 Ret(l.X + r.X,
				 l.Y + r.Y);

	return Ret;
}

inline Vector2 operator-(const Vector2& l, const Vector2& r)
{
	Vector2 Ret(l.X - r.X,
				 l.Y - r.Y);

	return Ret;
}

inline Vector2 operator*(const Vector2& l, float f)
{
	Vector2 Ret(l.X * f,
				 l.Y * f);

	return Ret;
}

inline Vector2 operator/(const Vector2& l, float f)
{
	Vector2 Ret(l.X / f,
				 l.Y / f);

	return Ret;
}

inline Vector2 operator/(float f, const Vector2& l)
{
	Vector2 Ret(f / l.X,
				 f / l.Y);

	return Ret;
}

struct ENGINE_API Vector3
{	
	union { float X, R; };
	union { float Y, G; };
	union { float Z, B; };
	// float X;
	// float Y;
	// float Z;

	Vector3()
	{
		X = Y = Z = 0;
	}

	Vector3(float _x, float _y, float _z)
	{
		X = _x;
		Y = _y;
		Z = _z;
	}

	Vector3(const float* pFloat)
	{
		X = pFloat[0];
		Y = pFloat[0];
		Z = pFloat[0];
	}

	Vector3(float f)
	{
		X = Y = Z = f;
	}

	Vector3(Vector2 V, float _z)
	{
		X = V.X;
		Y = V.Y;
		Z = _z;
	}
	
	Vector3(glm::vec3 V)
	{
		X = V.x;
		Y = V.y;
		Z = V.z;
	}

	glm::vec3 ToGLM() const
	{
		return glm::vec3(X, Y, Z);
	}

	Vector3& operator+=(const Vector3& r)
	{
		X += r.X;
		Y += r.Y;
		Z += r.Z;

		return *this;
	}

	Vector3& operator-()
	{
		X *= -1;
		Y *= -1;
		Z *= -1;

		return *this;
	}

	Vector3& operator-=(const Vector3& r)
	{
		X -= r.X;
		Y -= r.Y;
		Z -= r.Z;

		return *this;
	}

	Vector3& operator*=(float f)
	{
		X *= f;
		Y *= f;
		Z *= f;

		return *this;
	}

	operator const float*() const
	{
		return &(X);
	}

	Vector3 Cross(const Vector3& V) const;

	float Dot(const Vector3& V) const;

	Vector3 GetNormalized() const;
	Vector3& Normalize();

	float Size() const;

	void Rotate(float Angle, const Vector3& Axis);
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
	
	friend std::ostream& operator<<(std::ostream& os, const Vector3& InVector);
	friend std::istream& operator>>(std::istream& is, Vector3& OutVector);
};

inline Vector3 operator+(const Vector3& l, const Vector3& r)
{
	Vector3 Ret(l.X + r.X,
				 l.Y + r.Y,
				 l.Z + r.Z);

	return Ret;
}

inline Vector3 operator-(const Vector3& l, const Vector3& r)
{
	Vector3 Ret(l.X - r.X,
				 l.Y - r.Y,
				 l.Z - r.Z);

	return Ret;
}

inline Vector3 operator*(const Vector3& l, float f)
{
	Vector3 Ret(l.X * f,
				 l.Y * f,
				 l.Z * f);

	return Ret;
}

inline Vector3 operator/(const Vector3& l, float f)
{
	Vector3 Ret(l.X / f,
				 l.Y / f,
				 l.Z / f);

	return Ret;
}

inline Vector3 operator/(float f, const Vector3& l)
{
	Vector3 Ret(f / l.X,
				 f / l.Y,
				 f / l.Z);

	return Ret;
}





struct ENGINE_API Vector4
{
	float X;
	float Y;
	float Z;
	float W;

	Vector4()
	{
	}

	Vector4(float _x, float _y, float _z, float _w)
	{
		X = _x;
		Y = _y;
		Z = _z;
		W = _w;
	}

	Vector4(glm::vec4 V)
	{
		X = V.x;
		Y = V.y;
		Z = V.z;
		W = V.w;
	}
	
	Vector4(Vector3 V, float _w)
	{
		X = V.X;
		Y = V.Y;
		Z = V.Z;
		W = _w;
	}
	glm::vec4 ToGLM() const
	{
		return glm::vec4(X, Y, Z, W);
	}
	
	void Print(bool endl = true) const
	{
		printf("(%.02f, %.02f, %.02f, %.02f)", X, Y, Z, W);

		if (endl)
		{
			printf("\n");
		}
	}

	Vector3 To3F() const
	{
		const Vector3 V(X, Y, Z);
		return V;
	}
};

inline Vector4 operator/(const Vector4& l, float f)
{
	Vector4 Ret(l.X / f,
	             l.Y / f,
	             l.Z / f,
	             l.W / f);

	return Ret;
}