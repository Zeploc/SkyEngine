// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Math/MathDefinitions.h"
// #include <matrix3x3.h>
// #include <matrix4x4.h>
// #include <glm/mat4x4.hpp>

struct PersProjInfo
{
	float FOV;
	float Width;
	float Height;
	float zNear;
	float zFar;
};

struct OrthoProjInfo
{
	float Right;
	float Left;
	float Bottom;
	float Top;
	float ZNear;
	float ZFar;
};

struct Quaternion;
template <typename T>
struct TVector3;
template <typename T>
struct TVector4;

class ENGINE_API Matrix4
{
public:
	float M[4][4];

	Matrix4()
	{
		InitIdentity();
	}

	// constructor from Assimp matrix
	Matrix4(const aiMatrix4x4& AssimpMatrix)
	{
		M[0][0] = AssimpMatrix.a1;
		M[0][1] = AssimpMatrix.a2;
		M[0][2] = AssimpMatrix.a3;
		M[0][3] = AssimpMatrix.a4;
		M[1][0] = AssimpMatrix.b1;
		M[1][1] = AssimpMatrix.b2;
		M[1][2] = AssimpMatrix.b3;
		M[1][3] = AssimpMatrix.b4;
		M[2][0] = AssimpMatrix.c1;
		M[2][1] = AssimpMatrix.c2;
		M[2][2] = AssimpMatrix.c3;
		M[2][3] = AssimpMatrix.c4;
		M[3][0] = AssimpMatrix.d1;
		M[3][1] = AssimpMatrix.d2;
		M[3][2] = AssimpMatrix.d3;
		M[3][3] = AssimpMatrix.d4;
	}
	
	Matrix4(const glm::mat4x4& GlmMatrix)
	{
		M[0][0] = GlmMatrix[0][0];
		M[0][1] = GlmMatrix[0][1];
		M[0][2] = GlmMatrix[0][2];
		M[0][3] = GlmMatrix[0][3];
		M[1][0] = GlmMatrix[1][0];
		M[1][1] = GlmMatrix[1][1];
		M[1][2] = GlmMatrix[1][2];
		M[1][3] = GlmMatrix[1][3];
		M[2][0] = GlmMatrix[2][0];
		M[2][1] = GlmMatrix[2][1];
		M[2][2] = GlmMatrix[2][2];
		M[2][3] = GlmMatrix[2][3];
		M[3][0] = GlmMatrix[3][0];
		M[3][1] = GlmMatrix[3][1];
		M[3][2] = GlmMatrix[3][2];
		M[3][3] = GlmMatrix[3][3];
	}

	Matrix4(const aiMatrix3x3& AssimpMatrix)
	{
		M[0][0] = AssimpMatrix.a1;
		M[0][1] = AssimpMatrix.a2;
		M[0][2] = AssimpMatrix.a3;
		M[0][3] = 0.0f;
		M[1][0] = AssimpMatrix.b1;
		M[1][1] = AssimpMatrix.b2;
		M[1][2] = AssimpMatrix.b3;
		M[1][3] = 0.0f;
		M[2][0] = AssimpMatrix.c1;
		M[2][1] = AssimpMatrix.c2;
		M[2][2] = AssimpMatrix.c3;
		M[2][3] = 0.0f;
		M[3][0] = 0.0f;
		M[3][1] = 0.0f;
		M[3][2] = 0.0f;
		M[3][3] = 1.0f;
	}

	Matrix4(float a00, float a01, float a02, float a03,
	         float a10, float a11, float a12, float a13,
	         float a20, float a21, float a22, float a23,
	         float a30, float a31, float a32, float a33)
	{
		M[0][0] = a00;
		M[0][1] = a01;
		M[0][2] = a02;
		M[0][3] = a03;
		M[1][0] = a10;
		M[1][1] = a11;
		M[1][2] = a12;
		M[1][3] = a13;
		M[2][0] = a20;
		M[2][1] = a21;
		M[2][2] = a22;
		M[2][3] = a23;
		M[3][0] = a30;
		M[3][1] = a31;
		M[3][2] = a32;
		M[3][3] = a33;
	}


	glm::mat4x4 ToGLM() const
	{
		glm::mat4x4 NewMatrix;
		NewMatrix[0][0] = M[0][0];
		NewMatrix[0][1] = M[0][1];
		NewMatrix[0][2] = M[0][2];
		NewMatrix[0][3] = M[0][3];
		NewMatrix[1][0] = M[1][0];
		NewMatrix[1][1] = M[1][1];
		NewMatrix[1][2] = M[1][2];
		NewMatrix[1][3] = M[1][3];
		NewMatrix[2][0] = M[2][0];
		NewMatrix[2][1] = M[2][1];
		NewMatrix[2][2] = M[2][2];
		NewMatrix[2][3] = M[2][3];
		NewMatrix[3][0] = M[3][0];
		NewMatrix[3][1] = M[3][1];
		NewMatrix[3][2] = M[3][2];
		NewMatrix[3][3] = M[3][3];
		return NewMatrix;
	}
	
	void SetZero()
	{
		//ZERO_MEM(m);
		memset(M, 0, sizeof(M));
	}

	Matrix4 Transpose() const
	{
		Matrix4 n;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				n.M[i][j] = M[j][i];
			}
		}

		return n;
	}

	void InitIdentity()
	{
		M[0][0] = 1.0f;
		M[0][1] = 0.0f;
		M[0][2] = 0.0f;
		M[0][3] = 0.0f;
		M[1][0] = 0.0f;
		M[1][1] = 1.0f;
		M[1][2] = 0.0f;
		M[1][3] = 0.0f;
		M[2][0] = 0.0f;
		M[2][1] = 0.0f;
		M[2][2] = 1.0f;
		M[2][3] = 0.0f;
		M[3][0] = 0.0f;
		M[3][1] = 0.0f;
		M[3][2] = 0.0f;
		M[3][3] = 1.0f;
	}

	Matrix4 operator*(const Matrix4& Right)
	{
		Matrix4 Ret;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				Ret.M[i][j] = M[i][0] * Right.M[0][j] +
					M[i][1] * Right.M[1][j] +
					M[i][2] * Right.M[2][j] +
					M[i][3] * Right.M[3][j];
			}
		}

		return Ret;
	}

	TVector4<float> operator*(const TVector4<float>& V) const;

	operator const float*() const
	{
		return &(M[0][0]);
	}

	void Print() const;

	float Determinant() const;

	Matrix4& Inverse();

	Matrix4 GetInverse();

	TVector3<float> GetViewLocation() const;
	TVector3<float> GetLocation() const;
	TVector3<float> GetForward() const;
	TVector3<float> GetRight() const;
	TVector3<float> GetUp() const;

	// TODO: Change all to not init but just apply (use identity first, default on construction?)
	void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);

	// TODO: Convert to "Set" instead of init, and confirm scale will not be affected/will be mantained
	void InitRotateTransform(float RotateX, float RotateY, float RotateZ);

	void InitRotateTransform(const Quaternion& quat);

	void SetViewTranslation(const TVector3<float> Location);
	void SetTranslationTransform(const TVector3<float> Location);
	void SetTranslationTransform(const float X, const float Y, const float Z);

	void SetCameraOrientation(TVector3<float> Forward, TVector3<float> Up);
	
	void SetLookAt(TVector3<float> Location, TVector3<float> Target, TVector3<float> Up);

	void SetPerspectiveProjection(const PersProjInfo& P);

	void SetOrthographicProjection(const OrthoProjInfo& P);
};